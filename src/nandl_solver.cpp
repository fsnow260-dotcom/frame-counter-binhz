#include "nandl_solver.hpp"
#include <algorithm>
#include <execution>
#include <ranges>

namespace nandl {

double Solver::erfc_inv(double y) {
    double t = std::sqrt(-2.0 * std::log(y / 2.0));
    for (int i = 0; i < 3; ++i) {
        double erfc_val = std::erfc(t);
        double pdf = (2.0 / std::sqrt(M_PI)) * std::exp(-t * t);
        t -= (erfc_val - y) / (-pdf);
    }
    return t;
}

double Solver::successProb(double windowMs, double sigmaMs) {
    return std::erf(windowMs / (2.0 * sigmaMs * std::sqrt(2.0)));
}

ModifierParams ModifierParams::compute(
    uint32_t totalInputs,
    double durationSec,
    uint32_t maxCPS,
    double hardestSectionPosition
) {
    ModifierParams p;
    p.nerveMultiplier = 1.0 + 0.20 * hardestSectionPosition;
    p.fatigueModifier = 1.0 + 0.05 * std::log2(1.0 + totalInputs / 500.0);
    if (maxCPS > 8.0) {
        p.cpsDrain = 1.0 + 0.10 * ((maxCPS - 8.0) / 8.0);
    } else {
        p.cpsDrain = 1.0;
    }
    return p;
}

SolverResult Solver::solve(
    std::span<const FrameWindow> windows,
    double targetProb,
    double levelDurationSec,
    const ModifierParams& mods
) {
    if (windows.empty() || levelDurationSec <= 0.0) {
        return {};
    }

    double pPerInput = std::pow(targetProb, 1.0 / static_cast<double>(windows.size()));
    double erfInv = erfc_inv(1.0 - pPerInput);
    double denom = 2.0 * std::sqrt(2.0) * erfInv;

    std::atomic<double> sumSigmaSq{0.0};
    std::atomic<uint32_t> tightCount{0};

    std::for_each(std::execution::par_unseq,
        windows.begin(), windows.end(),
        [&](const FrameWindow& w) {
            double sigma = w.windowMs / denom;
            sumSigmaSq.fetch_add(sigma * sigma, std::memory_order_relaxed);
            if (w.windowMs < 1.0) {
                tightCount.fetch_add(1, std::memory_order_relaxed);
            }
        });

    double rmsSigma = std::sqrt(sumSigmaSq.load() / static_cast<double>(windows.size()));
    double basePrecision = rmsSigma / levelDurationSec;

    double adjustedSigma = basePrecision * mods.nerveMultiplier
                         * mods.fatigueModifier * mods.cpsDrain;

    return SolverResult{
        .sigmaPerSecond = basePrecision,
        .sigmaPerSecondNerve = adjustedSigma,
        .basePrecision = basePrecision,
        .nerveMultiplier = mods.nerveMultiplier,
        .fatigueModifier = mods.fatigueModifier,
        .cpsDrain = mods.cpsDrain,
        .totalInputs = static_cast<uint32_t>(windows.size()),
        .tightWindows = tightCount.load(),
        .levelDurationSec = levelDurationSec
    };
}

void AsyncSolver::requestSolve(
    std::vector<FrameWindow> windows,
    double targetProb,
    double durationSec,
    const ModifierParams& mods
) {
    if (m_worker.joinable()) {
        m_cancelFlag.store(true);
        m_worker.join();
    }
    m_cancelFlag.store(false);

    m_worker = std::jthread([this, ws = std::move(windows),
                              tp = targetProb, ds = durationSec, mp = mods]
                            (std::stop_token st) {
        auto result = Solver::solve(ws, tp, ds, mp);
        if (!st.stop_requested()) {
            std::lock_guard lock(m_resultMutex);
            m_latestResult = result;
            m_hasResult.store(true, std::memory_order_release);
        }
    });
}

std::optional<SolverResult> AsyncSolver::pollResult() {
    if (!m_hasResult.load(std::memory_order_acquire)) return std::nullopt;
    std::lock_guard lock(m_resultMutex);
    m_hasResult.store(false, std::memory_order_release);
    return m_latestResult;
}

} // namespace nandl
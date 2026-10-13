#pragma once
#include <vector>
#include <span>
#include <atomic>
#include <mutex>
#include <optional>
#include <thread>
#include <cmath>

namespace nandl {

struct FrameWindow {
    double windowMs;
    double clickBoundary;
    double releaseBoundary;
    uint32_t inputIndex;
    uint32_t frameIndex;
};

struct ModifierParams {
    double nerveMultiplier = 1.15;
    double fatigueModifier = 1.05;
    double cpsDrain = 1.08;

    static ModifierParams compute(
        uint32_t totalInputs,
        double durationSec,
        uint32_t maxCPS,
        double hardestSectionPosition
    );
};

struct SolverResult {
    double sigmaPerSecond = 0.0;
    double sigmaPerSecondNerve = 0.0;
    double basePrecision = 0.0;
    double nerveMultiplier = 0.0;
    double fatigueModifier = 0.0;
    double cpsDrain = 0.0;
    uint32_t totalInputs = 0;
    uint32_t tightWindows = 0;
    double levelDurationSec = 0.0;
};

class Solver {
public:
    static double erfc_inv(double y);
    static double successProb(double windowMs, double sigmaMs);

    static SolverResult solve(
        std::span<const FrameWindow> windows,
        double targetProb,
        double levelDurationSec,
        const ModifierParams& mods
    );
};

class AsyncSolver {
public:
    void requestSolve(
        std::vector<FrameWindow> windows,
        double targetProb,
        double durationSec,
        const ModifierParams& mods
    );

    std::optional<SolverResult> pollResult();

private:
    std::jthread m_worker;
    std::atomic<bool> m_cancelFlag{false};
    std::atomic<bool> m_hasResult{false};
    std::mutex m_resultMutex;
    SolverResult m_latestResult{};
};

} // namespace nandl
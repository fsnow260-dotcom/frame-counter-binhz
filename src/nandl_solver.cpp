#include "nandl_solver.hpp"

namespace nandl {
SolverResult Solver::solve(
    const std::vector<FrameWindow>& windows,
    double targetProb,
    double levelDurationSec
) {
    if (windows.empty() || levelDurationSec <= 0.0) return {};

    // Xác suất thành công cho mỗi input
    double pPerInput = std::pow(targetProb, 1.0 / windows.size());

    // Tính toán đơn giản hóa để minh họa logic
    // Trong thực tế, NaN GD sử dụng các mô hình phức tạp hơn (erfc, v.v.)
    double sumSigmaSq = 0.0;
    for (const auto& w : windows) {
        // Công thức ước lượng: sigma ~ window / (2 * sqrt(2) * erfc_inv(1-p))
        // Ở đây ta dùng một giá trị đơn giản hóa
        double sigma = w.windowMs / (2.0 * std::sqrt(2.0) * 2.0); // Giả định
        sumSigmaSq += sigma * sigma;
    }

    double rmsSigma = std::sqrt(sumSigmaSq / windows.size());
    double basePrecision = rmsSigma / levelDurationSec;

    return SolverResult{
        .sigmaPerSecond = basePrecision, // Sẽ được điều chỉnh bởi các yếu tố khác
        .basePrecision = basePrecision,
        .totalInputs = (uint32_t)windows.size()
    };
}
}
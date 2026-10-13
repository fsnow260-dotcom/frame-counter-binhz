#pragma once
#include <vector>
#include <cmath>

namespace nandl {
struct FrameWindow {
    double windowMs;      // Độ rộng cửa sổ (tính bằng ms)
    uint32_t frameIndex;  // Chỉ số frame trong level
};

struct SolverResult {
    double sigmaPerSecond; // Chỉ số σ/s
    double basePrecision;  // Độ chính xác cơ bản
    uint32_t totalInputs;  // Tổng số input
};

class Solver {
public:
    static SolverResult solve(
        const std::vector<FrameWindow>& windows,
        double targetProb,
        double levelDurationSec
    );
};
}
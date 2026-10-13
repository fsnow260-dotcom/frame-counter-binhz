#pragma once

namespace nandl {

struct SolverConfig {
    double targetProb = 0.99;
    double nerveMultiplier = 1.15;
    double fatigueModifier = 1.05;
    double cpsDrain = 1.08;

    static SolverConfig& get() {
        static SolverConfig instance;
        return instance;
    }
};

} // namespace nandl
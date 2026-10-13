#pragma once
namespace nandl {
struct SolverConfig {
    double targetProb = 0.99;
    static SolverConfig& get() {
        static SolverConfig instance;
        return instance;
    }
};
}
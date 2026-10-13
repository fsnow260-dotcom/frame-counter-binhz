#pragma once
#include "nandl_solver.hpp"
#include <filesystem>
#include <vector>
#include <Geode/utils/file.hpp>

namespace nandl {

class ExportEngine {
public:
    static geode::Result<> exportFWC(
        const SolverResult& result,
        const std::vector<FrameWindow>& windows,
        const std::filesystem::path& outPath
    );

    static geode::Result<> exportJSON(
        const SolverResult& result,
        const std::vector<FrameWindow>& windows,
        const std::filesystem::path& outPath
    );

private:
    template<typename T>
    static void appendLE(std::vector<uint8_t>& buf, T val) {
        const auto* p = reinterpret_cast<const uint8_t*>(&val);
        buf.insert(buf.end(), p, p + sizeof(T));
    }
};

} // namespace nandl
#include "export_engine.hpp"
#include <Geode/utils/matjson.hpp>

namespace nandl {

geode::Result<> ExportEngine::exportFWC(
    const SolverResult& result,
    const std::vector<FrameWindow>& windows,
    const std::filesystem::path& outPath
) {
    std::vector<uint8_t> buf;
    buf.reserve(64 + windows.size() * 24);

    const char magic[4] = {'F','W','C','1'};
    buf.insert(buf.end(), magic, magic + 4);

    appendLE<uint32_t>(buf, result.totalInputs);
    appendLE<double>(buf, result.levelDurationSec);
    appendLE<double>(buf, result.sigmaPerSecond);
    appendLE<double>(buf, result.sigmaPerSecondNerve);
    appendLE<uint32_t>(buf, result.tightWindows);

    for (auto& w : windows) {
        appendLE<double>(buf, w.windowMs);
        appendLE<uint32_t>(buf, w.frameIndex);
        appendLE<uint32_t>(buf, w.inputIndex);
    }

    return geode::utils::file::writeBinary(outPath, buf);
}

geode::Result<> ExportEngine::exportJSON(
    const SolverResult& result,
    const std::vector<FrameWindow>& windows,
    const std::filesystem::path& outPath
) {
    matjson::Value root;
    root["version"] = "1.0.0";
    root["solver"] = "NaNDL";
    root["result"]["sigmaPerSecond"] = result.sigmaPerSecond;
    root["result"]["sigmaPerSecondNerve"] = result.sigmaPerSecondNerve;
    root["result"]["basePrecision"] = result.basePrecision;
    root["result"]["nerveMultiplier"] = result.nerveMultiplier;
    root["result"]["fatigueModifier"] = result.fatigueModifier;
    root["result"]["cpsDrain"] = result.cpsDrain;
    root["result"]["totalInputs"] = result.totalInputs;
    root["result"]["tightWindows"] = result.tightWindows;

    auto& windowsArr = root["windows"];
    for (auto& w : windows) {
        matjson::Value entry;
        entry["windowMs"] = w.windowMs;
        entry["frameIndex"] = w.frameIndex;
        entry["inputIndex"] = w.inputIndex;
        windowsArr.push(std::move(entry));
    }

    return geode::utils::file::writeString(outPath, root.dump(2));
}

} // namespace nandl
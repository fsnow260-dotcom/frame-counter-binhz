#include <Geode/utils/file.hpp>
#include <matjson.hpp>
// #include <msgpack.hpp> // Cần cho .gdr

// Hàm giả lập đọc file .json
std::optional<std::vector<nandl::FrameWindow>> parseJSONReplay(const std::filesystem::path& path) {
    auto content = geode::utils::file::readString(path);
    if (!content) return std::nullopt;

    auto json = matjson::parse(*content);
    if (!json) return std::nullopt;

    std::vector<nandl::FrameWindow> windows;
    // Giả sử file JSON có cấu trúc: { "inputs": [ { "frame": 120, "window": 2.5 }, ... ] }
    if (json->contains("inputs")) {
        for (auto& input : json->at("inputs")) {
            nandl::FrameWindow w;
            w.frameIndex = input.at("frame").asInt().unwrap();
            w.windowMs = input.at("window").asDouble().unwrap();
            windows.push_back(w);
        }
    }
    return windows;
}
#pragma once
#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include <cstdint>

namespace nandl {

struct ReplayInput {
    double timestampMs;
    bool down;
    int button;
    bool player1;
};

struct Replay {
    std::vector<ReplayInput> inputs;
    double levelDurationMs = 0.0;
    std::string levelID;
    std::string playerName;
    uint32_t frameRate = 240;
};

class ReplayParser {
public:
    static std::optional<Replay> parse(const std::filesystem::path& path);

private:
    static std::optional<Replay> parseGDR(const std::filesystem::path& path);
    static std::optional<Replay> parseSLC(const std::filesystem::path& path);
    static std::optional<Replay> parseJSON(const std::filesystem::path& path);
    static std::optional<Replay> parseFWC(const std::filesystem::path& path);

    template<typename T>
    static T readLE(const uint8_t* p) {
        T val;
        std::memcpy(&val, p, sizeof(T));
        return val;
    }

    static std::optional<std::string> readFile(const std::filesystem::path& p);
    static std::optional<std::vector<uint8_t>> readBinary(const std::filesystem::path& p);
};

} // namespace nandl
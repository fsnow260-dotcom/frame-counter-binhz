#include "replay_parser.hpp"
#include <fstream>
#include <algorithm>
#include <cstring>

namespace nandl {

std::optional<Replay> ReplayParser::parse(const std::filesystem::path& path) {
    auto ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    if (ext == ".gdr")  return parseGDR(path);
    if (ext == ".slc")  return parseSLC(path);
    if (ext == ".json") return parseJSON(path);
    if (ext == ".fwc")  return parseFWC(path);
    return std::nullopt;
}

std::optional<Replay> ReplayParser::parseGDR(const std::filesystem::path& path) {
    auto data = readFile(path);
    if (!data) return std::nullopt;
    // Placeholder: implement JSON/msgpack parsing using matjson
    return std::nullopt;
}

std::optional<Replay> ReplayParser::parseSLC(const std::filesystem::path& path) {
    auto buf = readBinary(path);
    if (!buf || buf->size() < 12) return std::nullopt;

    const uint8_t* p = buf->data();
    uint32_t magic = readLE<uint32_t>(p); p += 4;
    if (magic != 0x534C43) return std::nullopt;

    uint32_t version = readLE<uint32_t>(p); p += 4;
    uint32_t count = readLE<uint32_t>(p); p += 4;

    Replay r;
    r.inputs.reserve(count);
    constexpr double TICK_MS = 1000.0 / 240.0;

    for (uint32_t i = 0; i < count && p + 5 <= buf->data() + buf->size(); ++i) {
        uint32_t tick = readLE<uint32_t>(p); p += 4;
        uint8_t flags = *p++;
        ReplayInput in;
        in.timestampMs = tick * TICK_MS;
        in.down = flags & 0x01;
        in.button = (flags >> 1) & 0x03;
        in.player1 = !(flags & 0x08);
        r.inputs.push_back(in);
    }
    return r;
}

std::optional<Replay> ReplayParser::parseJSON(const std::filesystem::path& path) {
    // Placeholder: use matjson::parse
    return std::nullopt;
}

std::optional<Replay> ReplayParser::parseFWC(const std::filesystem::path& path) {
    auto buf = readBinary(path);
    if (!buf || buf->size() < 16) return std::nullopt;

    const uint8_t* p = buf->data();
    const uint8_t* end = buf->data() + buf->size();

    if (std::memcmp(p, "FWC1", 4) != 0) return std::nullopt;
    p += 4;

    Replay r;
    uint32_t count = readLE<uint32_t>(p); p += 4;
    r.levelDurationMs = readLE<double>(p); p += 8;

    uint32_t idLen = readLE<uint32_t>(p); p += 4;
    if (p + idLen > end) return std::nullopt;
    r.levelID.assign(reinterpret_cast<const char*>(p), idLen);
    p += idLen;

    r.inputs.reserve(count);
    for (uint32_t i = 0; i < count && p + 11 <= end; ++i) {
        ReplayInput in;
        in.timestampMs = readLE<double>(p); p += 8;
        in.down = (*p++) & 0x01;
        in.button = (*p++) & 0x03;
        in.player1 = (*p++) & 0x01;
        ++p;
        r.inputs.push_back(in);
    }
    return r;
}

std::optional<std::string> ReplayParser::readFile(const std::filesystem::path& p) {
    std::ifstream f(p, std::ios::binary);
    if (!f) return std::nullopt;
    return std::string(std::istreambuf_iterator<char>(f),
                       std::istreambuf_iterator<char>());
}

std::optional<std::vector<uint8_t>> ReplayParser::readBinary(const std::filesystem::path& p) {
    std::ifstream f(p, std::ios::binary | std::ios::ate);
    if (!f) return std::nullopt;
    auto size = f.tellg();
    f.seekg(0);
    std::vector<uint8_t> buf(size);
    f.read(reinterpret_cast<char*>(buf.data()), size);
    return buf;
}

} // namespace nandl
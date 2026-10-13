#pragma once
#include <Geode/binding/FMODAudioEngine.hpp>
#include <unordered_map>
#include <filesystem>

namespace nandl {

enum class AudioCue {
    TightWindow,
    CriticalWindow,
    SolveComplete,
    NewRecord
};

class AudioEngine {
public:
    static AudioEngine& get() {
        static AudioEngine instance;
        return instance;
    }

    void loadCue(AudioCue cue, const std::filesystem::path& oggPath) {
        auto id = FMODAudioEngine::sharedEngine()->playEffect(
            oggPath.string().c_str(), 0.0f, 0.0f, 0.0f
        );
        m_cueMap[cue] = id;
    }

    void trigger(AudioCue cue, float volume = 1.0f) {
        auto it = m_cueMap.find(cue);
        if (it == m_cueMap.end()) return;
        FMODAudioEngine::sharedEngine()->playEffect(
            it->second, volume, 0.0f, 0.0f
        );
    }

private:
    std::unordered_map<AudioCue, int> m_cueMap;
};

} // namespace nandl
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace nandl {

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;

struct InputEvent {
    TimePoint timestamp;
    bool down;
    int button;
    bool player1;
    uint64_t frameIndex;
};

class InputQueue {
public:
    static InputQueue& get() {
        static InputQueue instance;
        return instance;
    }

    void enqueue(const InputEvent& ev) {
        std::lock_guard lock(m_mutex);
        m_queue.push(ev);
    }

    template <typename Fn>
    void processUpTo(TimePoint cutoff, Fn&& callback) {
        std::lock_guard lock(m_mutex);
        while (!m_queue.empty() && m_queue.front().timestamp <= cutoff) {
            callback(m_queue.front());
            m_queue.pop();
        }
    }

private:
    std::mutex m_mutex;
    std::queue<InputEvent> m_queue;
};

struct FrameState {
    static inline std::atomic<TimePoint> currentFrameStart{};
    static inline std::atomic<TimePoint> previousFrameStart{};
    static inline std::atomic<uint64_t> frameIndex{0};
    static inline std::atomic<bool> inputEnabled{true};
    static inline std::atomic<double> lastFrameDeltaMs{16.6667};

    static constexpr double PHYSICS_TPS = 240.0;
    static constexpr double SUBFRAME_MS = 1000.0 / PHYSICS_TPS;
};

} // namespace nandl

#if defined(GEODE_IS_WINDOWS)
class $modify(CBFEGLView, cocos2d::CCEGLView) {
    void pollEvents() {
        nandl::FrameState::previousFrameStart.store(
            nandl::FrameState::currentFrameStart.load(std::memory_order_relaxed)
        );
        nandl::FrameState::currentFrameStart.store(
            nandl::Clock::now(), std::memory_order_release
        );

        auto prev = nandl::FrameState::previousFrameStart.load();
        auto curr = nandl::FrameState::currentFrameStart.load();
        if (prev.time_since_epoch().count() != 0) {
            auto delta = std::chrono::duration<double, std::milli>(curr - prev).count();
            nandl::FrameState::lastFrameDeltaMs.store(delta, std::memory_order_relaxed);
        }

        nandl::FrameState::frameIndex.fetch_add(1, std::memory_order_relaxed);
        cocos2d::CCEGLView::pollEvents();
    }
};
#else
class $modify(CBFScheduler, cocos2d::CCScheduler) {
    void update(float dt) {
        nandl::FrameState::previousFrameStart.store(
            nandl::FrameState::currentFrameStart.load(std::memory_order_relaxed)
        );
        nandl::FrameState::currentFrameStart.store(
            nandl::Clock::now(), std::memory_order_release
        );
        nandl::FrameState::frameIndex.fetch_add(1, std::memory_order_relaxed);
        cocos2d::CCScheduler::update(dt);
    }
};
#endif

class $modify(CBFInputGate, GJBaseGameLayer) {
    void handleButton(bool down, int button, bool isPlayer1) {
        if (!nandl::FrameState::inputEnabled.load(std::memory_order_acquire)) {
            nandl::InputQueue::get().enqueue({
                .timestamp = nandl::Clock::now(),
                .down = down,
                .button = button,
                .player1 = isPlayer1,
                .frameIndex = nandl::FrameState::frameIndex.load()
            });
            return;
        }
        GJBaseGameLayer::handleButton(down, button, isPlayer1);
    }
};

class $modify(CBFPhysicsSplit, PlayerObject) {
    void update(float dt) {
        static thread_local bool inSubstep = false;
        if (inSubstep) {
            PlayerObject::update(dt);
            return;
        }

        auto& queue = nandl::InputQueue::get();
        auto frameDelta = nandl::FrameState::lastFrameDeltaMs.load();
        int substeps = std::max(1, static_cast<int>(std::ceil(frameDelta / nandl::FrameState::SUBFRAME_MS)));

        double substepDt = dt / static_cast<double>(substeps);

        for (int i = 0; i < substeps; ++i) {
            auto subframeTime = nandl::FrameState::currentFrameStart.load()
                + std::chrono::duration_cast<nandl::Clock::duration>(
                    std::chrono::duration<double, std::milli>(i * nandl::FrameState::SUBFRAME_MS)
                  );

            queue.processUpTo(subframeTime, [&](const nandl::InputEvent& ev) {
                inSubstep = true;
                nandl::FrameState::inputEnabled.store(true, std::memory_order_release);
                PlayerObject::update(substepDt);
                nandl::FrameState::inputEnabled.store(false, std::memory_order_release);
                inSubstep = false;
            });

            PlayerObject::update(substepDt);
        }
    }
};
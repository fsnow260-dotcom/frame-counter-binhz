#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/ui/Notification.hpp>
#include "SolverConfig.hpp"
#include "ui/SolverPopup.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    auto mod = Mod::get();

    // ── Đọc setting ban đầu ────────────────────────────────
    auto& cfg = nandl::SolverConfig::get();
    cfg.targetProb      = mod->getSettingValue<double>("target-prob");
    cfg.nerveMultiplier = mod->getSettingValue<double>("nerve-multiplier");
    cfg.fatigueModifier = mod->getSettingValue<double>("fatigue-multiplier");
    cfg.cpsDrain        = mod->getSettingValue<double>("cps-drain");

    log::info("NaNDL loaded: prob={}, nerve={}, fatigue={}, cps={}",
        cfg.targetProb, cfg.nerveMultiplier,
        cfg.fatigueModifier, cfg.cpsDrain);

    // ── Lắng nghe thay đổi slider ──────────────────────────
    listenForSettingChanges<double>("target-prob", [](double v) {
        nandl::SolverConfig::get().targetProb = v;
    });
    listenForSettingChanges<double>("nerve-multiplier", [](double v) {
        nandl::SolverConfig::get().nerveMultiplier = v;
    });
    listenForSettingChanges<double>("fatigue-multiplier", [](double v) {
        nandl::SolverConfig::get().fatigueModifier = v;
    });
    listenForSettingChanges<double>("cps-drain", [](double v) {
        nandl::SolverConfig::get().cpsDrain = v;
    });

    // ── Mở popup bằng bool toggle ──────────────────────────
    listenForSettingChanges<bool>("open-solver-popup", [](bool v) {
        if (!v) return;

        if (auto popup = nandl::SolverPopup::create()) {
            popup->show();
        } else {
            log::warn("Failed to create SolverPopup");
        }

        // Tự tắt toggle để lần sau bật lại vẫn trigger
        Mod::get()->setSettingValue("open-solver-popup", false);
    });
}
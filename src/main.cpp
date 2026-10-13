#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Event.hpp>
#include "ui/SolverPopup.hpp"
#include "SolverConfig.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    auto mod = Mod::get();

    auto& cfg = nandl::SolverConfig::get();
    cfg.targetProb       = mod->getSettingValue<double>("target-prob");
    cfg.nerveMultiplier  = mod->getSettingValue<double>("nerve-multiplier");
    cfg.fatigueModifier  = mod->getSettingValue<double>("fatigue-multiplier");
    cfg.cpsDrain         = mod->getSettingValue<double>("cps-drain");

    listenForSettingChanges<double>("target-prob",
        [](double v) { nandl::SolverConfig::get().targetProb = v; });
    listenForSettingChanges<double>("nerve-multiplier",
        [](double v) { nandl::SolverConfig::get().nerveMultiplier = v; });
    listenForSettingChanges<double>("fatigue-multiplier",
        [](double v) { nandl::SolverConfig::get().fatigueModifier = v; });
    listenForSettingChanges<double>("cps-drain",
        [](double v) { nandl::SolverConfig::get().cpsDrain = v; });

    ButtonSettingPressedEventV3(mod->getID(), "open-solver")
        .listen([](auto*, auto*) {
            nandl::SolverPopup::create()->show();
        }).leak();

    ButtonSettingPressedEventV3(mod->getID(), "reset-solver")
        .listen([](auto*, auto*) {
            auto m = Mod::get();
            m->setSettingValue("target-prob", 0.99);
            m->setSettingValue("nerve-multiplier", 1.15);
            m->setSettingValue("fatigue-multiplier", 1.05);
            m->setSettingValue("cps-drain", 1.08);
            Notification::create("Solver reset", NotificationIcon::Success)->show();
        }).leak();
}
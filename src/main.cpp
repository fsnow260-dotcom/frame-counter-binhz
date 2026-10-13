#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Event.hpp>
#include "SolverConfig.hpp"
#include "ui/SolverPopup.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    auto mod = Mod::get();
    auto& cfg = nandl::SolverConfig::get();
    cfg.targetProb = mod->getSettingValue<double>("target-prob");

    listenForSettingChanges<double>("target-prob", [](double v) {
        nandl::SolverConfig::get().targetProb = v;
    });

    listenForSettingChanges<bool>("open-solver-popup", [](bool v) {
        if (!v) return;
        if (auto popup = nandl::SolverPopup::create()) popup->show();
        Mod::get()->setSettingValue("open-solver-popup", false);
    });
}
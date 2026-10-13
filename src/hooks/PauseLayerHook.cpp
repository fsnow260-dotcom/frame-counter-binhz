#include <Geode/modify/PauseLayer.hpp>
#include "../ui/SolverPopup.hpp"

using namespace geode::prelude;

class $modify(NaNDLPauseLayer, PauseLayer) {
    void customSetup() override {
        PauseLayer::customSetup();

        auto rightMenu = this->getChildByID("right-button-menu");
        if (!rightMenu) {
            log::warn("right-button-menu not found in PauseLayer");
            return;
        }

        auto logo = CCSprite::create("logo.png"_spr);
        if (!logo) {
            logo = CCSprite::createWithSpriteFrameName("geode.loader/logo.png");
        }
        logo->setScale(0.6f);

        auto btn = CCMenuItemSpriteExtra::create(
            logo,
            this,
            menu_selector(NaNDLPauseLayer::onOpenPrecisionHUD)
        );
        btn->setID("nandl-precision-button"_spr);
        rightMenu->addChild(btn);
        rightMenu->updateLayout();
    }

    void onOpenPrecisionHUD(CCObject*) {
        nandl::SolverPopup::create()->show();
    }
};
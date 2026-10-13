#include "SolverPopup.hpp"

namespace nandl {

SolverPopup* SolverPopup::create() {
    auto ret = new SolverPopup();
    if (ret && ret->init(400.f, 320.f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SolverPopup::setup() {
    this->setTitle("NaNDL Precision Solver");

    auto& cfg = SolverConfig::get();

    auto probLabel = cocos2d::CCLabelBMFont::create(
        "Target Probability", "goldFont.fnt");
    probLabel->setScale(0.6f);
    m_mainLayer->addChildAtPosition(probLabel, geode::Anchor::Top,
        {0.f, -55.f});

    auto slider = geode::SliderNode::create(
        [this](double val) {
            m_targetProb = val;
            m_probValueLabel->setString(
                fmt::format("{:.4f}", val).c_str()
            );
        },
        geode::SliderNode::createDefaultStyle(),
        0.50, 0.9999, cfg.targetProb
    );
    slider->setPosition({m_size.width / 2, m_size.height - 90.f});
    m_mainLayer->addChild(slider);

    m_probValueLabel = cocos2d::CCLabelBMFont::create(
        fmt::format("{:.4f}", cfg.targetProb).c_str(), "chatFont.fnt");
    m_probValueLabel->setScale(0.7f);
    m_mainLayer->addChildAtPosition(m_probValueLabel,
        geode::Anchor::Top, {0.f, -115.f});

    auto modTitle = cocos2d::CCLabelBMFont::create(
        "Active Modifiers", "goldFont.fnt");
    modTitle->setScale(0.6f);
    m_mainLayer->addChildAtPosition(modTitle, geode::Anchor::Top,
        {0.f, -150.f});

    auto toggleMenu = cocos2d::CCMenu::create();
    toggleMenu->setLayout(
        geode::RowLayout::create()
            ->setGap(12.f)
            ->setAxisAlignment(geode::AxisAlignment::Center)
    );

    auto makeToggle = [&](const char* name, bool& flag) {
        auto lbl = cocos2d::CCLabelBMFont::create(name, "chatFont.fnt");
        lbl->setScale(0.65f);
        auto tog = CCMenuItemToggler::create(
            cocos2d::CCSprite::createWithSpriteFrameName(
                "GJ_checkOff_001.png"),
            cocos2d::CCSprite::createWithSpriteFrameName(
                "GJ_checkOn_001.png"),
            this, menu_selector(SolverPopup::onToggle)
        );
        tog->setUserObject(lbl);
        tog->toggle(flag);
        toggleMenu->addChild(tog);
    };

    makeToggle("K_T Nerve", m_nerveEnabled);
    makeToggle("K_U Fatigue", m_fatigueEnabled);
    makeToggle("K_C CPS", m_cpsEnabled);

    toggleMenu->updateLayout();
    toggleMenu->setPosition({m_size.width / 2, m_size.height - 190.f});
    m_mainLayer->addChild(toggleMenu);

    auto applySpr = geode::ButtonSprite::create(
        "Apply", "goldFont.fnt", "GJ_button_01.png", 0.8f);
    auto applyBtn = CCMenuItemSpriteExtra::create(
        applySpr, this, menu_selector(SolverPopup::onApply));
    auto applyMenu = cocos2d::CCMenu::create();
    applyMenu->addChild(applyBtn);
    applyMenu->setPosition({m_size.width / 2, 35.f});
    m_mainLayer->addChild(applyMenu);

    return true;
}

void SolverPopup::onToggle(CCObject* sender) {
    auto tog = static_cast<CCMenuItemToggler*>(sender);
    auto lbl = static_cast<cocos2d::CCLabelBMFont*>(tog->getUserObject());
    if (!lbl) return;

    auto str = std::string(lbl->getString());
    if (str == "K_T Nerve") m_nerveEnabled = !tog->isToggled();
    else if (str == "K_U Fatigue") m_fatigueEnabled = !tog->isToggled();
    else if (str == "K_C CPS") m_cpsEnabled = !tog->isToggled();
}

void SolverPopup::onApply(CCObject*) {
    auto mod = geode::Mod::get();
    mod->setSavedValue("target-prob", m_targetProb);
    mod->setSavedValue("nerve-enabled", m_nerveEnabled);
    mod->setSavedValue("fatigue-enabled", m_fatigueEnabled);
    mod->setSavedValue("cps-enabled", m_cpsEnabled);

    auto& cfg = SolverConfig::get();
    cfg.targetProb = m_targetProb;
    cfg.nerveMultiplier = m_nerveEnabled ? 1.15 : 1.0;
    cfg.fatigueModifier = m_fatigueEnabled ? 1.05 : 1.0;
    cfg.cpsDrain = m_cpsEnabled ? 1.08 : 1.0;

    geode::Notification::create(
        "Solver settings applied", geode::NotificationIcon::Success
    )->show();

    this->onClose(nullptr);
}

} // namespace nandl
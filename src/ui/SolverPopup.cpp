#include "SolverPopup.hpp"

using namespace geode::prelude;

namespace nandl {

SolverPopup* SolverPopup::create() {
    auto ret = new SolverPopup();
    if (ret && ret->init(360.f, 240.f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SolverPopup::setup() {
    this->setTitle("NaNDL Precision Solver");

    auto& cfg = SolverConfig::get();
    m_targetProb = cfg.targetProb;

    // ── Label: Target Probability ─────────────────────────
    auto probLabel = cocos2d::CCLabelBMFont::create(
        "Target Probability", "goldFont.fnt");
    probLabel->setScale(0.6f);
    m_mainLayer->addChildAtPosition(probLabel, Anchor::Top, {0.f, -50.f});

    // ── Slider ────────────────────────────────────────────
    auto slider = SliderNode::create(
        [this](double val) {
            m_targetProb = val;
            if (m_probValueLabel) {
                m_probValueLabel->setString(
                    fmt::format("{:.4f}", val).c_str()
                );
            }
        },
        SliderNode::createDefaultStyle(),
        0.50, 0.9999, cfg.targetProb
    );
    slider->setPosition({m_size.width / 2, m_size.height - 95.f});
    m_mainLayer->addChild(slider);

    // ── Label hiển thị giá trị hiện tại ────────────────────
    m_probValueLabel = cocos2d::CCLabelBMFont::create(
        fmt::format("{:.4f}", cfg.targetProb).c_str(), "chatFont.fnt");
    m_probValueLabel->setScale(0.7f);
    m_mainLayer->addChildAtPosition(
        m_probValueLabel, Anchor::Top, {0.f, -125.f});

    // ── Nút Apply ─────────────────────────────────────────
    auto applySpr = ButtonSprite::create(
        "Apply", "goldFont.fnt", "GJ_button_01.png", 0.8f);
    auto applyBtn = CCMenuItemSpriteExtra::create(
        applySpr, this, menu_selector(SolverPopup::onApply));
    auto applyMenu = cocos2d::CCMenu::create();
    applyMenu->addChild(applyBtn);
    applyMenu->setPosition({m_size.width / 2, 30.f});
    m_mainLayer->addChild(applyMenu);

    return true;
}

void SolverPopup::onApply(CCObject*) {
    auto& cfg = SolverConfig::get();
    cfg.targetProb = m_targetProb;

    Notification::create(
        "Solver setting applied",
        NotificationIcon::Success
    )->show();

    this->onClose(nullptr);
}

} // namespace nandl
#include "hud_overlay.hpp"
#include <Geode/ui/GeodeUI.hpp>

namespace nandl {

PrecisionHUD* PrecisionHUD::create() {
    auto ret = new PrecisionHUD();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool PrecisionHUD::init() {
    if (!CCNode::init()) return false;

    auto panel = cocos2d::CCSprite::create("GJ_square01.png");
    panel->setScaleX(2.5f);
    panel->setScaleY(1.8f);
    panel->setOpacity(180);
    this->addChild(panel, -1);

    auto title = cocos2d::CCLabelBMFont::create("NaNDL Precision", "bigFont.fnt");
    title->setScale(0.5f);
    title->setPosition({0, 38});
    this->addChild(title);

    m_sigmaLabel = cocos2d::CCLabelBMFont::create("σ/s: --.--", "chatFont.fnt");
    m_sigmaLabel->setScale(0.7f);
    m_sigmaLabel->setPosition({-60, 10});
    m_sigmaLabel->setAnchorPoint({0, 0.5f});
    this->addChild(m_sigmaLabel);

    m_windowLabel = cocos2d::CCLabelBMFont::create("Window: --.--ms", "chatFont.fnt");
    m_windowLabel->setScale(0.7f);
    m_windowLabel->setPosition({-60, -10});
    m_windowLabel->setAnchorPoint({0, 0.5f});
    this->addChild(m_windowLabel);

    m_tightIndicator = cocos2d::CCSprite::create("circle.png");
    m_tightIndicator->setScale(0.3f);
    m_tightIndicator->setPosition({-80, -10});
    m_tightIndicator->setColor({100, 255, 100});
    this->addChild(m_tightIndicator);

    this->scheduleUpdate();
    return true;
}

void PrecisionHUD::setSolver(AsyncSolver* s) {
    // Optional: external solver injection
}

void PrecisionHUD::update(float dt) {
    auto result = m_solver.pollResult();
    if (result) {
        m_latestResult = *result;
        m_sigmaLabel->setString(
            fmt::format("σ/s: {:.2f}", result->sigmaPerSecond).c_str()
        );

        if (result->tightWindows > 0) {
            m_tightIndicator->setColor({255, 80, 80});
            m_windowLabel->setString(
                fmt::format("Tight: {} windows <1ms", result->tightWindows).c_str()
            );
        } else {
            m_tightIndicator->setColor({100, 255, 100});
            m_windowLabel->setString("All windows >1ms");
        }
    }
}

} // namespace nandl
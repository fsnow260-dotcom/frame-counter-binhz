#pragma once
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/SliderNode.hpp>
#include <Geode/ui/Layout.hpp>
#include "../SolverConfig.hpp"

namespace nandl {

class SolverPopup : public geode::Popup {
public:
    static SolverPopup* create();

protected:
    bool setup() override;
    void onToggle(CCObject* sender);
    void onApply(CCObject*);

private:
    double m_targetProb = 0.99;
    bool m_nerveEnabled = true;
    bool m_fatigueEnabled = true;
    bool m_cpsEnabled = true;
    cocos2d::CCLabelBMFont* m_probValueLabel = nullptr;
};

} // namespace nandl
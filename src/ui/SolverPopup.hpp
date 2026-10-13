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
    void onApply(CCObject* sender);

private:
    double m_targetProb = 0.99;
    cocos2d::CCLabelBMFont* m_probValueLabel = nullptr;
};

} // namespace nandl
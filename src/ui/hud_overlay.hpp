#pragma once
#include <cocos2d.h>
#include "../nandl_solver.hpp"

namespace nandl {

class PrecisionHUD : public cocos2d::CCNode {
public:
    static PrecisionHUD* create();
    bool init() override;
    void update(float dt) override;
    void setSolver(AsyncSolver* s);

private:
    cocos2d::CCLabelBMFont* m_sigmaLabel = nullptr;
    cocos2d::CCLabelBMFont* m_windowLabel = nullptr;
    cocos2d::CCSprite* m_tightIndicator = nullptr;
    AsyncSolver m_solver;
    SolverResult m_latestResult{};
};

} // namespace nandl
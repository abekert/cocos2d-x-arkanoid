//
//  HUD.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 8/7/16.
//
//

#pragma once

#include "cocos2d.h"

#include "../../Extensions/ColorSprite.hpp"

class HudDelegate {
public:
    virtual void gamePaused() {};
    virtual void gameResumed() {};
    virtual void exitGame() {};
};

//class CCSprite;

class HUD : public cocos2d::CCLayer {
public:
    CREATE_FUNC(HUD);
    
    void setScores(int scores);
    int getScores() { return scores; }
    void incrementScores() { setScores(scores + 1); }
    HudDelegate *delegate;
    
protected:
    virtual bool init();
private:
    CCMenu* menu;
    
    cocos2d::CCMenuItemImage *pauseButton;
    bool paused;
    void pauseButtonCallback();
    
    cocos2d::CCMenuItemLabel *exitButton;
    void presentExitButton();
    void exitButtonCallback();
    
    CCSprite *black;
    void setupBlackScreen();
    void fadeBlackTo(int opacity, float duration);
    
    int scores;
    cocos2d::CCLabelTTF *scoresLabel;
};

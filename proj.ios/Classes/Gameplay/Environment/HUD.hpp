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
    ~HUD();
    
    void setScores(int scores);
    int getScores() { return scores; }
    void incrementScores() { setScores(scores + 1); }
    
    void setLives(int lives);
    int getLives() { return lives; }
    void decrementLives() { setLives(lives - 1); }
    
    void presentLevel(int levelNumber, float duration);
    void presentGameOver();
    
    void setTopColor(cocos2d::ccColor3B color);
    void setTopColor(cocos2d::ccColor3B color, float duration);
    
    HudDelegate *delegate;
    
protected:
    virtual bool init();
private:
    CCMenu* menu;
    float normalFontSize;
    float bigFontSize;
    
    void addTopPanel();
    
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
    int lives;
    cocos2d::CCLabelTTF *scoresLabel;
    cocos2d::CCLabelTTF *livesLabel;
    
    cocos2d::CCLabelTTF *bigLabel;
    bool isPresenting;
    void endPresentation();
};

//
//  MainMenuScene.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 21/07/16.
//
//

#pragma once

#include "cocos2d.h"

class MainMenuScene : public cocos2d::CCLayerColor
{
public:
    static cocos2d::CCScene* scene();
    virtual bool init();
    
    CREATE_FUNC(MainMenuScene);
    
private:
    void onEnter();
    
    void startButtonCallback(CCObject* sender);
    void toGameScene();
    
    void animateLabelsAppear();
    void addLetterToLabel(CCNode* labelNode, void *letterPointer);
    void showCredits();

    cocos2d::CCMenu *menu;
    cocos2d::CCMenuItemImage *startButton;
    cocos2d::CCLabelTTF *arkanoidLabel;
    cocos2d::CCLabelTTF *creditsLabel;
    
    void addSoundButton();
    cocos2d::CCMenuItemImage *soundButton;
    void soundButtonCallback(CCObject* sender);
};

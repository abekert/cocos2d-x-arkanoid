//
//  MainMenuScene.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 21/07/16.
//
//

#pragma once

#include "cocos2d.h"

class MainMenuScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(MainMenuScene);
    
private:
    // a start button callback
    void startButtonCallback(CCObject* sender);
    void toGameScene();
};

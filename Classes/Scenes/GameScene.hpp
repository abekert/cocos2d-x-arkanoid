//
//  GameScene.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 22/07/16.
//
//

#pragma once

#include "cocos2d.h"

class Level;
class Raquet;

class GameScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void testAction(CCObject* pSender);
    void menuCloseCallback(CCObject* pSender);
    void delayHideCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);
    ~GameScene();
private:
    Level *level;
    Raquet *raquet;
    
    void setupLevel();
    void setupRaquet();

    bool raquetMovingEnabled;
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);

};

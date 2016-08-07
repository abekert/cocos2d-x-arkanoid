//
//  GameScene.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 22/07/16.
//
//

#pragma once

#include "cocos2d.h"
#include "../Gameplay/GamePhysics.hpp"
#include "../Gameplay/Models/Level.hpp"

//class Level;
class Raquet;
class Ball;

class Backlight;

class GameScene : public cocos2d::CCLayer, public GamePhysicsDelegate, public LevelCompleteDelegate
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
    Ball *ball;
    GamePhysics *physics;
    
    Backlight *backlight;
    
    void setupLevel(float presentationDuration);
    void setupRaquet();
    void addBall();
    void setupPhysics();
    
    void setupBacklight();
    
    void startGame(float delay);
    void startGame();

    void update(float dt);
    
    virtual void ballTouchedBottomEdge();
    void levelComplete();

    bool raquetMovingEnabled;
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);

};

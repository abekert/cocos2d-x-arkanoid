//
//  MainMenuScene.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 21/07/16.
//
//

#pragma once

#include "cocos2d.h"
#include "Box2D.h"

class Backlight;

class MainMenuScene : public cocos2d::CCLayer, public b2ContactListener
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
    
    b2World *world; // World with physic
    b2Body *ballBody ; // Body of the ball
    b2BodyDef bodyDef; // Define the above Body
    b2FixtureDef fixtureDef; // Define some static features: friction, restitution, density, etc.
    b2CircleShape bodyShape; // the shape of body
    
    cocos2d::CCSprite *ball; // Ball shape
    float deltaTime; // The variable to calculate time
    
    void addWall(float width,float height); // Create the Wall edge around the screen for the ball to collide with.
    
    void update(float dt); // Update scene by time

private:
    cocos2d::CCMenuItemImage *pCloseItem;
    
    
    Backlight* backlight;
    void setupLoose();
    // a start button callback
    void startButtonCallback(CCObject* sender);
    void toGameScene();
    
        virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    
    void BeginContact(b2Contact* contact);

};

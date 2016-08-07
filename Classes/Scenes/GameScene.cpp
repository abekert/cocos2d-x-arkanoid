//
//  GameScene.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 22/07/16.
//
//

#include "GameScene.hpp"
#include "../Gameplay/Models/LevelPresenter.hpp"
#include "../Gameplay/Models/Raquet.hpp"
#include "../Gameplay/Models/Ball.hpp"

#include "../Gameplay/Environment/Backlight.hpp"

USING_NS_CC;

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
        
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN + 1, true);
    
    float duration = 2;
    
    setupLevel(duration);
    setupRaquet();
    addBall();
    setupBacklight();
    setupPhysics();
    
    scheduleUpdate();
    startGame(duration);
    
    return true;
}

void GameScene::setupLevel(float presentationDuration) {
    Level::setupDefaults();
    level = Level::createSampleLevel(2, 2);
    level->delegate = this;
//        LevelPresenter::presentLevelMoveStyle(level, this, 1, 10);
    auto presenter = new LevelPresenter;
    presenter->presentLevelLineByLine(level, this, presentationDuration, 10);
}

void GameScene::setupRaquet() {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    raquet = Raquet::create(250, 20);
    this->addChild(raquet);
    raquet->setY(visibleSize.height * 0.12f);
    raquet->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + raquet->getY()));
    raquet->resetMoving();
    raquetMovingEnabled = true;
}

void GameScene::addBall() {
    CCPoint position = raquet->getPosition();
    position.y += 50;
    ball = Ball::create(12);
    ball->setPosition(position);
    this->addChild(ball);
}

void GameScene::setupPhysics() {
    physics = new GamePhysics(level, raquet, ball);
    physics->delegate = this;
}

void GameScene::setupBacklight() {
    backlight = Backlight::addTo(this, -100);
    backlight->sprite->setColor(ccc3(255, 0, 0));
    backlight->sprite->setOpacity(0);
}

GameScene::~GameScene() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void GameScene::menuCloseCallback(CCObject* pSender)
{
    auto block = level->getBlock(1, 2);
    block->setVisible(!block->isVisible());
}

void GameScene::delayHideCallback(CCObject* pSender)
{
    auto block = level->getBlock(1, 2);
    block->setVisible(1, false);
}

void GameScene::testAction(CCObject* pSender)
{
    auto block = level->getBlock(1, 2);
    auto rotate = CCRotateBy::create(1, 180);
    block->runAction(rotate);
    CCLog("rotate");
}

void GameScene::startGame(float delay) {
    auto wait = CCDelayTime::create(delay);
    auto start = CCCallFunc::create(this, callfunc_selector(GameScene::startGame));
    this->runAction(CCSequence::createWithTwoActions(wait, start));
}

void GameScene::startGame() {
    CCPoint position = raquet->getPosition();
    position.y += 50;
    physics->setBallPosition(position);
    physics->setBallSpeed(10);
    physics->setBallDirectionBottom();
}

#pragma mark Touch Input

bool GameScene::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    auto touchPosition = touch->getLocation();
    
    if (raquetMovingEnabled) {
        raquet->handleTouchAtPosition(touchPosition);
    }

//    physics->setBallPosition(touchPosition, true);

    return true;
}

void GameScene::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    auto touchPosition = touch->getLocationInView();
    
    if (raquetMovingEnabled) {
        raquet->handleTouchAtPosition(touchPosition);
    }
}

void GameScene::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    printf("ccTouchEnded");
}
void GameScene::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    printf("ccTouchCancelled");
}

#pragma mark Update

void GameScene::update(float dt) {
    if (physics) {
        physics->update(dt);
    }
}

#pragma mark Physics Delegate

void GameScene::ballTouchedBottomEdge() {
    if (backlight) {
        backlight->blink(1);
    }
}

#pragma mark Level Complete Delegate

void GameScene::levelComplete() {
    CCLOG("Completed!");
    
    if (!backlight) {
        return;
    }
    
    backlight->setColor(5, 0, 255, 0);
    backlight->fadeIn(2);
}




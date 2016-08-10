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
#include "../Gameplay/Environment/Colors.hpp"

USING_NS_CC;

static const int defaultLivesCount = 3;

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
    colorPalette = Colors::flatAndHipPalette();
    auto bc = colorPalette->background;
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(bc.r, bc.g, bc.b, 255)))
    {
        return false;
    }
    
    lives = defaultLivesCount;
    levelNumber = 1;
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    setColor(colorPalette->background);
    
    setupRaquet();
    addBall();
    setupBacklight();
    setupHUD();
    
    scheduleUpdate();
    prepareToStartGame();
    
    return true;
}

void GameScene::setupLevel(float presentationDuration) {
    Level::setupDefaults();
    if (level) {
        level->removeFromParent();
    }
    level = Level::createSimpleLevel(1 + levelNumber, 1 + levelNumber, colorPalette->blocks);
    level->delegate = this;
    auto presenter = new LevelPresenter;
//    presenter->presentLevelMoveStyle(level, this, presentationDuration, 10);
    presenter->presentLevelLineByLine(level, this, presentationDuration, 10);
//    level->setPositionY(level->getPositionY() - 50);
}

void GameScene::setupRaquet() {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    float width = 0.22 * visibleSize.width;
    float height = 0.075 * width;
    raquet = Raquet::create(width, height, colorPalette->racquet);
    this->addChild(raquet, 10);
    raquet->setY(visibleSize.height * 0.12f);
    raquet->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + raquet->getY()));
    raquet->resetMoving();
}

void GameScene::addBall() {
    CCPoint position = raquet->getPosition();
    auto radius = raquet->getContentSize().height * 0.6f;
    position.y += 4 * radius;
    ball = Ball::create(radius, colorPalette->ball);
    ball->setPosition(position);
    this->addChild(ball, 10);
}

void GameScene::setupPhysics() {
    if (physics) {
        delete physics;
    }
    physics = new GamePhysics(level, raquet, ball);
    physics->delegate = this;
}

void GameScene::setupHUD() {
    hud = HUD::create();
    hud->setLives(lives);
    hud->setTopColor(colorPalette->topPanelColor);
    hud->delegate = this;
    this->addChild(hud, 100);
}

void GameScene::setupBacklight() {
    backlight = Backlight::addTo(this, 1);
    backlight->sprite->setColor(colorPalette->backlightFail);
    backlight->sprite->setOpacity(0);
}

GameScene::~GameScene() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

#pragma mark ----

void GameScene::prepareToStartGame() {
    auto duration = 2;
    paused = true;
    raquetMovingEnabled = false;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    // Colorize background
    auto color = colorPalette->background;
    auto colorize = CCTintTo::create(duration, color.r, color.g, color.b);
    this->runAction(colorize);

    // Move raquet to start position
    raquet->moveToStartPosition();
    
    // Move ball to start position
    float ballY = raquet->getY() + ball->getRadius() * 4;
    float ballX = visibleSize.width * 0.5f;
    auto moveBall = CCMoveTo::create(duration * 0.5f, ccp(ballX, ballY));
    ball->runAction(moveBall);
    
    // Setup Level
    setupLevel(duration * 0.5f);

    // Setup Physics
    setupPhysics();
    
    // Present Level Number
    hud->presentLevel(levelNumber, duration);
    
    startGame(duration);
}

void GameScene::startGame(float delay) {
    auto wait = CCDelayTime::create(delay);
    auto start = CCCallFunc::create(this, callfunc_selector(GameScene::startGame));
    this->runAction(CCSequence::createWithTwoActions(wait, start));
}

void GameScene::startGame() {
    physics->setBallPosition(ball->getPosition());
    physics->setBallSpeed(10 + levelNumber * 1.5f);
    physics->setBallDirectionBottom();
    paused = false;
    raquetMovingEnabled = true;
}

void GameScene::gameOver() {
    if (backlight) {
        backlight->fadeIn(0.25f);
    }

    if (hud) {
        hud->presentGameOver();
    }
    
    raquetMovingEnabled = false;
    paused = true;
}


#pragma mark Touch Input

bool GameScene::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    auto touchPosition = touch->getLocation();
    
    if (raquetMovingEnabled) {
        raquet->handleTouchAtPosition(touchPosition);
    }

    return true;
}

void GameScene::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    auto touchPosition = touch->getLocationInView();
    
    if (raquetMovingEnabled) {
        raquet->handleTouchAtPosition(touchPosition);
    }
}

void GameScene::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
//    printf("ccTouchEnded");
}
void GameScene::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
//    printf("ccTouchCancelled");
}


#pragma mark Update

void GameScene::update(float dt) {
    if (physics && !paused) {
        physics->update(dt);
    }
}


#pragma mark Physics Delegate

void GameScene::ballTouchedBottomEdge() {
    lives -= 1;

    if (hud) {
        hud->setLives(lives);
    }
    
    if (lives <= 0) {
        gameOver();
        return;
    }
    
    if (backlight) {
        backlight->blink(1);
    }
}


#pragma mark Level Complete Delegate

void GameScene::destroyedBlock() {
    scores += 1;
    if (hud) {
        hud->setScores(scores);
    }
}

void GameScene::levelComplete() {
    if (!backlight) {
        return;
    }

    auto color = colorPalette->backlightSuccess;
    auto colorize = CCTintTo::create(0.5f, color.r, color.g, color.b);
    
    levelNumber += 1;
    auto startNextLevel = CCCallFunc::create(this, callfunc_selector(GameScene::prepareToStartGame));
    
    this->runAction(CCSequence::createWithTwoActions(colorize, startNextLevel));
    
    paused = true;
    raquetMovingEnabled = false;
}


#pragma mark HUD Delegate

void GameScene::gamePaused() {
    paused = true;
    raquetMovingEnabled = false;
}

void GameScene::gameResumed() {
    paused = false;
    raquetMovingEnabled = true;
}

void GameScene::exitGame() {
    CCDirector::sharedDirector()->popScene();
}


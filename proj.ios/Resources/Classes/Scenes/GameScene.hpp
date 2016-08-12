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
#include "../Gameplay/Environment/HUD.hpp"

class Raquet;
class Ball;
class Colors;
class Backlight;

class GameScene : public cocos2d::CCLayerColor, public GamePhysicsDelegate, public LevelCompleteDelegate, public HudDelegate
{
public:
    virtual bool init();
    
    static cocos2d::CCScene* scene();
        
    CREATE_FUNC(GameScene);
    ~GameScene();
    
    typedef enum {
        StatePresentingLevel,
        StateRunning,
        StatePaused,
        StateScored
    } GameStates;
    GameStates getState() { return gameState; }
    
private:
    Level *level;
    Raquet *raquet;
    Ball *ball;
    GamePhysics *physics;
    HUD *hud;
    
    Colors *colorPalette;
    Backlight *backlight;
    
    bool paused;
    GameStates gameState;
    int scores;
    int lives;
    int levelNumber;
    
    void setupLevel(float presentationDuration);
    void setupRaquet();
    void addBall();
    void setupPhysics();
    void setupHUD();
    
    void setupBacklight();
    
    void prepareToStartGame();
    void prepareBallAndRaquet(float moveDuration);
    void startGame(float delay);
    void startGame();
    void gameOver();

    void update(float dt);
    
    virtual void ballTouchedBottomEdge();
    
    virtual void levelComplete();
    virtual void destroyedBlock();
    
    virtual void gamePaused();
    virtual void gameResumed();
    virtual void exitGame();

    bool raquetMovingEnabled;
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);

};

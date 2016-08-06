//
//  GamePhysicsDelegate.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 02/08/16.
//
//

#pragma once

#include "Box2D.h"
#include "cocos2d.h"

class Level;
class Block;
class Raquet;
class Ball;
class Backlight;


class GamePhysicsDelegate {
public:
    virtual void ballTouchedBottomEdge() {};
};

class GamePhysics : public b2ContactListener
{
public:
    GamePhysics(Level *level, Raquet *raquet, Ball *ball);

    void update(float dt);

    void setBallPosition(const cocos2d::CCPoint &position, bool resetForce = false);
    void setBallSpeed(float speed);
    void setBallDirectionBottom();
    
    GamePhysicsDelegate* delegate;
private:
    Level *level;
    Raquet *raquet;
    Ball *ball;
    
    b2World *world; // World with physic
    b2Body *ballBody;
    b2Body *raquetBody;
    
    b2Fixture *bottomEdge;
    
    b2Body *addBall(Ball *ball);
    void setupRaquet();
    void updateRaquetPosition();
    void setupLevel();
    
    void synchronizePhysicsWithScene();
    void applyPhysicsToNode(cocos2d::CCNode *node, b2Body *body);
    Backlight* backlight;
    void setupLoose();
    
    void addWall(float w,float h);
    
    virtual void BeginContact(b2Contact* contact);
    void beginContactBallAndBlock(b2Body *ballBody, Ball *ball, b2Body *blockBody, Block *block);
    void beginContactBallAndRaquet(b2Body *ballBody, Ball *ball, b2Body *raquetBody, Raquet *raquet);

    struct BlockAndBody {
        Block *block;
        b2Body *body;
    };
    std::list<BlockAndBody *> destroyingBlocks;
    void checkDestroyingBlocks();
    std::list<b2Body *> bodiesToDelete;
    void deletePhysicBodies();
};
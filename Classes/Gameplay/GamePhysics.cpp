//
//  GamePhysicsDelegate.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 02/08/16.
//
//

#include "GamePhysics.hpp"

#include "Models/Level.hpp"
#include "Models/Raquet.hpp"
#include "Models/Ball.hpp"

#include "../Snippets.h"

#define SCALE_RATIO 32.0

typedef enum {
    CategoryBall = 1,
    CategoryRaquet = 1 << 1,
    CategoryBlock = 1 << 2,
    CategoryActiveEnvironment = 1 << 3,
    CategoryInactiveEnvironment = 1 << 4
} Categories;

typedef struct {
    CCNode *data;
    Categories category;
    bool synchronizeSpriteAndBody = false;
} UserData;

GamePhysics::GamePhysics(Level *level, Raquet *raquet, Ball *ball) {
    this->level = level;
    this->raquet = raquet;
    this->ball = ball;
    
    b2Vec2 gravity = b2Vec2(0, 0);
    world = new b2World(gravity);
    world->SetContactListener(this);
    
    ballBody = addBall(ball);
    setupRaquet();
    setupLevel();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    addWall(visibleSize.width, visibleSize.height);
    
    bodiesToDelete = *new std::list<b2Body *>();
    destroyingBlocks = *new std::list<BlockAndBody *>();
}

void GamePhysics::addWall(float w,float h) {
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0,0);
    
    b2Body *groundBody = world->CreateBody(&groundBodyDef);
    b2EdgeShape groundEdge;
    b2FixtureDef boxShapeDef;
    boxShapeDef.filter.categoryBits = CategoryActiveEnvironment;
    boxShapeDef.shape = &groundEdge;
    
    groundEdge.Set(b2Vec2(0,0), b2Vec2(w/SCALE_RATIO, 0));
    bottomEdge = groundBody->CreateFixture(&boxShapeDef);
    
    groundEdge.Set(b2Vec2(0,0), b2Vec2(0,w/SCALE_RATIO));
    groundBody->CreateFixture(&boxShapeDef);
    
    groundEdge.Set(b2Vec2(0, h/SCALE_RATIO), b2Vec2(w/SCALE_RATIO, h/SCALE_RATIO));
    groundBody->CreateFixture(&boxShapeDef);
    
    groundEdge.Set(b2Vec2(w/SCALE_RATIO, h/SCALE_RATIO), b2Vec2(w/SCALE_RATIO, 0));
    groundBody->CreateFixture(&boxShapeDef);
}

#pragma mark b2ContactListener

void GamePhysics::BeginContact(b2Contact* contact) {
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();
    
    if (fixtureA == bottomEdge || fixtureB == bottomEdge) {
        if (delegate) {
            delegate->ballTouchedBottomEdge();
        }
        return;
    }
    
    b2Body *bodyA = fixtureA->GetBody();
    b2Body *bodyB = fixtureB->GetBody();
    
    UserData *userDataA = (UserData *)bodyA->GetUserData();
    UserData *userDataB = (UserData *)bodyB->GetUserData();
    
    if (!userDataA || !userDataB) {
        return;
    }
    
    // Ball And Block
    
    if (userDataA->category == CategoryBall && userDataB->category == CategoryBlock) {
        userDataB->synchronizeSpriteAndBody = true;
        beginContactBallAndBlock(bodyA, (Ball *)userDataA->data, bodyB, (Block *)userDataB->data);
        return;
    }
    
    if (userDataB->category == CategoryBall && userDataA->category == CategoryBlock) {
        userDataA->synchronizeSpriteAndBody = true;
        beginContactBallAndBlock(bodyB, (Ball *)userDataB->data, bodyA, (Block *)userDataA->data);
        return;
    }
    
    // Ball And Raquet
    
    if (userDataA->category == CategoryBall && userDataB->category == CategoryRaquet) {
        beginContactBallAndRaquet(bodyA, (Ball *)userDataA->data, bodyB, (Raquet *)userDataB->data);
        return;
    }
    
    if (userDataB->category == CategoryBall && userDataA->category == CategoryRaquet) {
        beginContactBallAndRaquet(bodyB, (Ball *)userDataB->data, bodyA, (Raquet *)userDataA->data);
        return;
    }

}

void GamePhysics::beginContactBallAndBlock(b2Body *ballBody, Ball *ball, b2Body *blockBody, Block *block) {
    
    bool destroyed = block->takeDamage();
    if (!destroyed) {
        return;
    }
    
    b2Filter filter;
    filter.categoryBits = CategoryInactiveEnvironment;
    filter.maskBits = 0;
    
    for (b2Fixture *fixture = blockBody->GetFixtureList(); fixture != NULL; fixture = fixture->GetNext()) {
        fixture->SetFilterData(filter);
    }
        
    auto blockAndBody = new BlockAndBody;
    blockAndBody->block = block;
    blockAndBody->body = blockBody;
    destroyingBlocks.push_back(blockAndBody);
}

void GamePhysics::beginContactBallAndRaquet(b2Body *ballBody, Ball *ball, b2Body *raquetBody, Raquet *raquet) {

    auto ballPosition = ball->getPosition();
    auto ballParent = ball->getParent();
    if (ballParent) {
        ballPosition = ballParent->convertToWorldSpace(ballPosition);
    }
    
    auto raquetPosition = raquet->getPosition();
    auto raquetParent = raquet->getParent();
    if (raquetParent) {
        raquetPosition = raquetParent->convertToWorldSpace(raquetPosition);
    }
    if (ballPosition.y < raquetPosition.y + raquet->getContentSize().height / 2 + ball->getRadius() / 2) {
        CCLOG("No special raquet-ball processing");
        return;
    }
    
    float forceX = 0.9f * ball->speed * raquet->reflectionForcePercentX(ballPosition);
    float forceY = sqrtf(sqr(ball->speed) - sqr(forceX));
    ballBody->SetLinearVelocity(b2Vec2(forceX, forceY));
}


#pragma mark Update Cycle

void GamePhysics::update(float deltaTime) {
    updateRaquetPosition();
    setBallSpeed(ball->speed);
    checkDestroyingBlocks();
    
    int positionIterations = 10;  // Location
    int velocityIterations = 10; // Velocity
    
    world->Step(deltaTime, velocityIterations, positionIterations);
    
    synchronizePhysicsWithScene();
    
//    world->ClearForces(); // Clear forces of Body
    world->DrawDebugData();  // Draw shapes to check collision in debug mode
    deletePhysicBodies();
}

void GamePhysics::checkDestroyingBlocks() {
    for (auto it = destroyingBlocks.begin(); it != destroyingBlocks.end(); ++it) {
        auto blockAndBody = *it;
        if (blockAndBody->block->getState() == Block::StateDestroyed) {
            bodiesToDelete.push_back(blockAndBody->body);
            destroyingBlocks.erase(it++);
//            CCLOG("Destroy block. Count left: %lu", destroyingBlocks.size());
        }
    }
}

void GamePhysics::deletePhysicBodies() {
    for (b2Body *body : bodiesToDelete) {
        if (!body) {
            continue;
        }
        world->DestroyBody(body);
    }
    bodiesToDelete.clear();
}

void GamePhysics::synchronizePhysicsWithScene() {
    for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext()) {

        UserData *userData = (UserData *)body->GetUserData();
        if (!userData) {
            continue;
        }
        
        if (!userData->synchronizeSpriteAndBody) {
            continue;
        }
        
        CCNode *node = (CCNode *)userData->data;
        if (node) {
            applyPhysicsToNode(node, body);
        }
    }
}
void GamePhysics::applyPhysicsToNode(cocos2d::CCNode *node, b2Body *body) {
    if (!node || !body) {
        return;
    }
    
    auto bodyPosition = body->GetPosition();
    auto position = ccp(SCALE_RATIO * bodyPosition.x, SCALE_RATIO * bodyPosition.y);
    auto parent = node->getParent();
    if (parent) {
        position = parent->convertToNodeSpace(position);
//        CCLog("Node position %0.2lf %0.2lf", position.x, position.y);
    }
    node->setPosition(position);
    node->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
}

#pragma mark Ball

b2Body * GamePhysics::addBall(Ball *ball) {
    b2CircleShape bodyShape;
    bodyShape.m_radius = ball->getRadius() / SCALE_RATIO; // The radius of body
    
    b2FixtureDef fixtureDef;
    fixtureDef.density=10;
    fixtureDef.friction=0;
    fixtureDef.restitution=1;
    fixtureDef.shape=&bodyShape; // point to bodyShape
    fixtureDef.filter.categoryBits = CategoryBall;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Dynamic collision
    
    bodyDef.position.Set(ball->getPosition().x/SCALE_RATIO,ball->getPosition().y/SCALE_RATIO);
    
    //ballBody
    auto ballBody = world->CreateBody(&bodyDef); // Create Body
    ballBody->CreateFixture(&fixtureDef); // Create static features
    
    UserData *data = new UserData;
    data->data = ball;
    data->category = CategoryBall;
    data->synchronizeSpriteAndBody = true;
    ballBody->SetUserData(data);
    
    return ballBody;
}

void GamePhysics::setBallPosition(const cocos2d::CCPoint &position, bool resetForce) {
    ball->setPosition(position);
    auto worldPosition = ball->getParent()->convertToWorldSpace(position);

    ballBody->SetTransform(b2Vec2(worldPosition.x / SCALE_RATIO, worldPosition.y / SCALE_RATIO), ballBody->GetAngle());
    if (resetForce) {
        ballBody->SetLinearVelocity(b2Vec2(0, 0));
        ballBody->SetAngularVelocity(0);
    }
}

void GamePhysics::setBallDirectionBottom() {
    ballBody->SetLinearVelocity(b2Vec2(0, -ball->speed));
}

void GamePhysics::setBallSpeed(float speed) {
    ball->speed = speed;
    auto velocity = ballBody->GetLinearVelocity();
    velocity.Normalize();
    velocity = speed * velocity;
    ballBody->SetLinearVelocity(velocity);
}

#pragma mark Raquet

void GamePhysics::setupRaquet() {
    auto size = raquet->getContentSize();
    b2PolygonShape bodyShape;
    bodyShape.SetAsBox(size.width / SCALE_RATIO / 2, size.height / SCALE_RATIO / 2);
    
    b2FixtureDef fixtureDef;
    fixtureDef.density=10;
    fixtureDef.friction=0;
    fixtureDef.restitution=1;
    fixtureDef.shape=&bodyShape;
    fixtureDef.filter.categoryBits = CategoryRaquet;

    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    
    auto position = raquet->getPosition();
    bodyDef.position.Set(position.x / SCALE_RATIO, position.y / SCALE_RATIO);
    
    //ballBody
    raquetBody = world->CreateBody(&bodyDef); // Create Body
    raquetBody->CreateFixture(&fixtureDef); // Create static features
    
    UserData *data = new UserData;
    data->data = raquet;
    data->category = CategoryRaquet;
    raquetBody->SetUserData(data);
}

void GamePhysics::updateRaquetPosition() {
    auto position = raquet->getPosition();
    raquetBody->SetTransform(b2Vec2(position.x / SCALE_RATIO, position.y / SCALE_RATIO), raquetBody->GetAngle());
}

#pragma mark Level

void GamePhysics::setupLevel() {
    for (int row = 0; row < level->getRowsNumber(); row++) {
        for (int column = 0; column < level->getColumnsNumber(); column++) {
//            auto block = level->getBlock(1, 1);
            auto block = level->getBlock(row, column);
    
            auto size = block->getContentSize();
            b2PolygonShape bodyShape;
            bodyShape.SetAsBox(size.width / SCALE_RATIO / 2, size.height / SCALE_RATIO / 2);
            
            b2FixtureDef fixtureDef;
            fixtureDef.density = 10;
            fixtureDef.friction = 0;
            fixtureDef.restitution = 1;
            fixtureDef.shape=&bodyShape; // point to bodyShape
            fixtureDef.filter.categoryBits = CategoryBlock;

            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody; // Dynamic collision
    
            auto position = block->getParent()->convertToWorldSpace(block->getPosition());
            bodyDef.position.Set(position.x / SCALE_RATIO, position.y / SCALE_RATIO);
            
            //ballBody
            auto body = world->CreateBody(&bodyDef); // Create Body
            body->CreateFixture(&fixtureDef); // Create static features
            body->SetLinearDamping(1);
            body->SetAngularDamping(1);
            
            UserData *data = new UserData;
            data->data = block;
            data->category = CategoryBlock;
            data->synchronizeSpriteAndBody = false;
            body->SetUserData(data);

            //            CCLog("block[%d][%d] position: (%0.2lf, %0.2lf) vertices[0]: (%0.2lf, %0.2lf)", row, column, bodyDef.position.x, bodyDef.position.y, bodyShape.GetVertex(0).x, bodyShape.GetVertex(0).y);
        }
    }
}

//
//  Ball.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 30/07/16.
//
//

#include "Ball.hpp"

Ball* Ball::create(float radius) {
    auto ball = new Ball();
    if (ball && ball->init()) {
        ball->autorelease();
        ball->radius = radius;
        ball->sprite = circleOfRadius(radius);
        ball->addChild(ball->sprite);
        return ball;
    }
    
    CC_SAFE_DELETE(ball);
    return NULL;
}

CCDrawNode* Ball::circleOfRadius(float radius) {
    CCDrawNode *draw = CCDrawNode::create();
    draw->drawDot(ccp(0, 0), radius, ccc4f(1, 1, 1, 1));
    return draw;
}

//void Ball::setPosition(const CCPoint &position) {
//    setPosition(position, true);
//}
//
//void Ball::setPosition(const CCPoint &position, bool ignorePhysics) {
//    needsPhysicsRecalculation = !ignorePhysics;
//    CCNode::setPosition(position);
//}
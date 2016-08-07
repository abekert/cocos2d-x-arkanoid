//
//  Ball.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 30/07/16.
//
//

#include "Ball.hpp"

Ball* Ball::create(float radius) {
    return create(radius, ccWHITE);
}

Ball* Ball::create(float radius, ccColor3B color) {
    auto ball = new Ball();
    if (ball && ball->init()) {
        ball->autorelease();
        ball->radius = radius;
        ball->sprite = circleOfRadius(radius, color);
        ball->addChild(ball->sprite);
        return ball;
    }
    
    CC_SAFE_DELETE(ball);
    return NULL;
}

CCDrawNode* Ball::circleOfRadius(float radius, ccColor3B color) {
    CCDrawNode *draw = CCDrawNode::create();
    draw->drawDot(ccp(0, 0), radius, ccc4FFromccc3B(color));
    return draw;
}


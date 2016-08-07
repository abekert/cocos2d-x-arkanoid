//
//  Ball.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 30/07/16.
//
//

#pragma once

#include "cocos2d.h"
USING_NS_CC;

class Ball : public CCNode
{
public:
    CREATE_FUNC(Ball);
    static Ball* create(float radius);
    static Ball* create(float radius, ccColor3B color);
    
    float getRadius() { return radius; }
    
    float speed;
private:
    CCDrawNode *sprite;
    float radius;
    
    static CCDrawNode* circleOfRadius(float radius, ccColor3B color);
};
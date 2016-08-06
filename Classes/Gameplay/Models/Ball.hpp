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
    float getRadius() { return radius; }
    
    float speed;
//    void setPosition(const CCPoint &position);
//    void setPosition(const CCPoint &position, bool ignorePhysics);
//    
//    bool needsPhysicsRecalculation = false;
private:
    CCDrawNode *sprite;
    float radius;
    
    static CCDrawNode* circleOfRadius(float radius);
};
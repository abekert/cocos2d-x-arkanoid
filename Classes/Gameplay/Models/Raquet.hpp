//
//  Raquet.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 29/07/16.
//
//

#pragma once

#include "cocos2d.h"
USING_NS_CC;

class Raquet : public CCNode
{
public:
    CREATE_FUNC(Raquet);
    static Raquet* create(float width, float height);
    
    void handleTouchAtPosition(CCPoint point);
    void resetMoving() { desiredPoint = getPosition(); }
    
    float speedX = 2000;
    
    float getY() { return y; }
    void setY(float newY);
    
    CCSize getContentSize() { return size; };
    
    void updateBordersX();
    
    /// A value in range [-1, 1]
    float reflectionForcePercentX(CCPoint ballPosition);
    
private:
    CCSprite *sprite;
    CCPoint desiredPoint;
    
    float y = 100;
    float xMin = 0;
    float xMax = 1000;
    void fitPositionIntoBordersX();

    static CCSprite* createBlankSprite(const ccColor4B& color, CCSize size);
    
    CCSize size;

    void update(float dt);
};
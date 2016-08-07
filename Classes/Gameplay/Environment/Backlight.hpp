//
//  Backlight.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 02/08/16.
//
//

#pragma once

#include "cocos2d.h"
USING_NS_CC;

class Backlight : public CCNode
{
public:
    CREATE_FUNC(Backlight);
    static Backlight* addTo(CCNode *layer, int zOrder = 0);

    CCSprite *sprite;
    
    void blink(float duration);
    void fadeIn(float duration);
    void fadeTo(int opacity, float duration);
    void fadeOut(float duration);
    void setColor(float duration, GLubyte red, GLubyte green, GLubyte blue);
    void setColor(float duration, ccColor3B color);

protected:
    virtual bool init();

private:
    const char *imageName = "loose-bottom.png";
};
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

protected:
    virtual bool init();

private:
    const char *imageName = "loose-bottom.png";
};
//
//  ColorSprite.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 8/9/16.
//
//

#pragma once

#include "cocos2d.h"

USING_NS_CC;

class ColorSprite : public cocos2d::CCSprite {
//    CREATE_FUNC(ColorSprite);
public:
    static CCSprite * create(const ccColor4B& color, CCSize size);
};
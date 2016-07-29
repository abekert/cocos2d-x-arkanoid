//
//  Block.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 24/07/16.
//
//

#pragma once

#include "cocos2d.h"
USING_NS_CC;

class Block : public CCNode
{
public:
    CREATE_FUNC(Block);
    static Block* create(float width, float height);
    void setVisible();
    void setVisible(bool visible);
    void setVisible(float delay, bool visible = true);
private:
    CCSprite* sprite;
    static CCSprite* createBlankSprite(const ccColor4B& color, CCSize size);
};
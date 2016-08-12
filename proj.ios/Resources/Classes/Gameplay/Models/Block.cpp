//
//  Block.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 24/07/16.
//
//

#include "Block.hpp"

#include "../../Extensions/ColorSprite.hpp"

float Block::destroyAnimationDuration = 1;

Block * Block::create(float width, float height) {
    return create(width, height, ccc3(255, 255, 255));
}

Block * Block::create(float width, float height, ccColor3B color) {
    auto block = new Block();
    if (block && block->init()) {
        block->autorelease();
        auto color4b = ccc4(color.r, color.g, color.b, 255);
        auto size = CCSize(width, height);
        block->sprite = ColorSprite::create(color4b, size);
        block->size = size;
        block->addChild(block->sprite);
        return block;
    }
    
    delete block;
    block = NULL;
    return NULL;
}

void Block::setVisible() {
    setVisible(true);
}

void Block::setVisible(bool visible) {
    CCNode::setVisible(visible);
    CCObject* nodeObject = NULL;
    CCARRAY_FOREACH(this->getChildren(), nodeObject)
    {
        CCNode* node = (CCNode *)nodeObject;
        node->setVisible(visible);
    }
}

void Block::setVisible(float delay, bool visible) {
    auto wait = CCDelayTime::create(delay);
    auto enable = CCCallFunc::create(this, callfunc_selector(Block::setVisible));
    sprite->runAction(CCSequence::createWithTwoActions(wait, enable));
}

bool Block::takeDamage() {
    auto fadeOut = CCFadeTo::create(destroyAnimationDuration, 0);
    setState(StateDestroyedPlayingAnimation);
    auto destroy = CCCallFunc::create(this, callfunc_selector(Block::setStateDestroyed));
    sprite->runAction(CCSequence::createWithTwoActions(fadeOut, destroy));
    
    return true;
}

void Block::fadeOut(float duration) {
    sprite->runAction(CCFadeTo::create(duration, 0));
}

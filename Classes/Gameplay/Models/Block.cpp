//
//  Block.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 24/07/16.
//
//

#include "Block.hpp"

Block* Block::create(float width, float height) {
    auto block = new Block();
    if (block && block->init()) {
        block->autorelease();
        block->sprite = Block::createBlankSprite(ccc4(200, 255, 255, 255), CCSize(width, height));
        block->addChild(block->sprite);
        return block;
    }
    
    delete block;
    block = NULL;
    return NULL;
}

CCSprite * Block::createBlankSprite(const ccColor4B& color, CCSize size)
{
    GLubyte *buffer = (GLubyte *)malloc(sizeof(GLubyte)*4);
    buffer[0] = color.r;
    buffer[1] = color.g;
    buffer[2] = color.b;
    buffer[3] = color.a;
    
    auto tex = new CCTexture2D();
    tex->initWithData(buffer, kCCTexture2DPixelFormat_RGBA8888, 1, 1, size);
    
    auto sprite = CCSprite::create();
    sprite->setTexture(tex);
    sprite->setTextureRect(CCRect(0, 0, size.width, size.height));
    
    tex->release();
    free(buffer);
    
    return sprite;
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

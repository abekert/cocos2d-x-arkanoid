//
//  Backlight.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 02/08/16.
//
//

#include "Backlight.hpp"

static const int fadeTag = 1;
static const int colorTag = 2;


Backlight* Backlight::addTo(CCNode *layer, int zOrder) {
    auto backlight = new Backlight();
    if (backlight && backlight->init()) {
        backlight->autorelease();

        layer->addChild(backlight, zOrder);
        
        return backlight;
    }
    
    CC_SAFE_DELETE(backlight);
    return NULL;

}

bool Backlight::init() {
    if (!CCNode::init()) {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    sprite = CCSprite::create(imageName);
    sprite->setScaleX(visibleSize.width);
    sprite->setScaleY(visibleSize.height / sprite->getContentSize().height);
    // position the sprite on the center of the screen
    sprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    addChild(sprite);
    
    return true;
}

void Backlight::blink(float duration) {
    auto fadeIn = CCFadeTo::create(duration * 0.2f, 255);
    auto wait = CCDelayTime::create(duration * 0.2f);
    auto fadeOut = CCFadeOut::create(duration * 0.6f);
    auto blinkAction = CCSequence::create(fadeIn, wait, fadeOut, NULL);
    blinkAction->setTag(fadeTag);
    
    sprite->stopActionByTag(fadeTag);
    sprite->runAction(blinkAction);
}

void Backlight::fadeIn(float duration) {
    auto fadeIn = CCFadeTo::create(duration, 255);
    fadeIn->setTag(fadeTag);
    sprite->stopActionByTag(fadeTag);
    sprite->runAction(fadeIn);
}

void Backlight::fadeOut(float duration) {
    auto fadeOut = CCFadeTo::create(duration, 0);
    fadeOut->setTag(fadeTag);
    sprite->stopActionByTag(fadeTag);
    sprite->runAction(fadeOut);
}

void Backlight::setColor(float duration, GLubyte red, GLubyte green, GLubyte blue) {
    auto colorize = CCTintTo::create(duration, red, green, blue);
    colorize->setTag(colorTag);
    sprite->stopActionByTag(colorTag);
    sprite->runAction(colorize);
}


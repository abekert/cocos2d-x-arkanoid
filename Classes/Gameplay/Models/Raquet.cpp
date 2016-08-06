//
//  Raquet.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 29/07/16.
//
//

#include "Raquet.hpp"

Raquet* Raquet::create(float width, float height) {
    auto raquet = new Raquet();
    if (raquet && raquet->init()) {
        raquet->autorelease();
        raquet->sprite = Raquet::createBlankSprite(ccc4(255, 255, 255, 255), CCSize(width, height));
        raquet->addChild(raquet->sprite);
        raquet->size = CCSize(width, height);
        raquet->updateBordersX();
        CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(raquet, 0, false);

        return raquet;
    }
    
    delete raquet;
    raquet = NULL;
    return NULL;
}

CCSprite* Raquet::createBlankSprite(const ccColor4B& color, CCSize size)
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

void Raquet::handleTouchAtPosition(CCPoint point) {
    desiredPoint = ccp(point.x, y);
}

void Raquet::setY(float newY) {
    y = newY;
    setPosition(ccp(getPositionX(), y));
}

void Raquet::update(float dt) {
    float x = getPositionX();
    if (x > desiredPoint.x) {
        x -= speedX * dt;
        x = MAX(x, desiredPoint.x);
    } else if (x < desiredPoint.x) {
        x += speedX * dt;
        x = MIN(x, desiredPoint.x);
    } else {
        x = desiredPoint.x;
    }
    x = clampf(x, xMin, xMax);
    setPosition(ccp(x, desiredPoint.y));
}

void Raquet::updateBordersX() {
    float width = sprite->getContentSize().width;
    float screenWidth = CCDirector::sharedDirector()->getVisibleSize().width;
    
    xMin = width * 0.5f;
    xMax = screenWidth - xMin;
    
    fitPositionIntoBordersX();
}

void Raquet::fitPositionIntoBordersX() {
    float x = clampf(getPositionX(), xMin, xMax);
    setPositionX(x);
}

float Raquet::reflectionForcePercentX(CCPoint ballPosition) {
    auto x = convertToNodeSpace(ballPosition).x;
    return clampf(2 * x / size.width, -1, 1);
}

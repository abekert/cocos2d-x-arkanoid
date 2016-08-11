//
//  ColorSprite.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 8/9/16.
//
//

#include "ColorSprite.hpp"

CCSprite * ColorSprite::create(const ccColor4B& color, CCSize size)
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

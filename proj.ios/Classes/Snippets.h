//
//  Snippets.h
//  Arkanoid
//
//  Created by Alexander Bekert on 8/6/16.
//
//

#pragma once

#include "cocos2d.h"

#define sqr(x) (x * x)

cocos2d::CCPoint getWorldPosition(cocos2d::CCNode *node) {
    auto position = node->getPosition();
    auto parent = node->getParent();
    if (parent) {
        position = parent->convertToWorldSpace(position);
    }
    return position;
}

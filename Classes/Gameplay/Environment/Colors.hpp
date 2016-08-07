//
//  Colors.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 8/7/16.
//
//

#pragma once

#include "cocos2d.h"

class Colors {
public:
    cocos2d::ccColor3B background;
    cocos2d::ccColor3B blocks;
    cocos2d::ccColor3B ball;
    cocos2d::ccColor3B racquet;
    
    cocos2d::ccColor3B backlightFail;
    cocos2d::ccColor3B backlightSuccess;
    
    /// http://flatcolors.net/palette/534-flat-hip#
    static Colors * flatAndHipPalette();
};
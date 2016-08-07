//
//  Colors.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 8/7/16.
//
//

#include "Colors.hpp"

USING_NS_CC;

Colors * Colors::flatAndHipPalette() {
    auto palette = new Colors();
    palette->background = ccc3(70, 36, 70);
    palette->blocks = ccc3(235, 107, 86);
    palette->ball = ccc3(255, 193, 83);
    palette->racquet = ccc3(255, 193, 83);
    
    palette->backlightFail = ccc3(176, 95, 109);
    palette->backlightSuccess = ccc3(71, 179, 157);
    
    return palette;
}
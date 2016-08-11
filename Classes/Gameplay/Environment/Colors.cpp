//
//  Colors.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 8/7/16.
//
//

#include "Colors.hpp"

USING_NS_CC;

static const char *paletteIndexKey = "colorPaletteIndex";

static ColorsManager *_sharedManager = NULL;

ColorsManager * ColorsManager::sharedManager() {
    if (!_sharedManager)
    {
        _sharedManager = new ColorsManager();
    }
    
    return _sharedManager;
}

ColorsManager::ColorsManager() {
    currentPaletteIndex = CCUserDefault::sharedUserDefault()->getIntegerForKey(paletteIndexKey, 0);
}

Colors * ColorsManager::getCurrentColorsPalette() {
    return Colors::colorsPaletteByIndex(currentPaletteIndex);
}

Colors * ColorsManager::nextColorsPalette() {
    currentPaletteIndex += 1;
    return getCurrentColorsPalette();
}

#pragma mark Palettes

Colors * Colors::flatAndHipPalette() {
    auto palette = new Colors();
    palette->background = ccc3(70, 36, 70);
    palette->blocks = ccc3(235, 107, 86);
    palette->ball = ccc3(255, 193, 83);
    palette->racquet = ccc3(255, 193, 83);
    
    palette->backlightFail = ccc3(176, 95, 109);
    palette->backlightSuccess = ccc3(71, 179, 157);
    
    palette->topPanelColor = palette->ball;
    
    return palette;
}

Colors * Colors::retroFlatPalette() {
    auto palette = new Colors();
    palette->background = ccc3(255, 252, 230);
    palette->blocks = ccc3(242, 77, 22);
    palette->ball = ccc3(237, 216, 52);
    palette->racquet = ccc3(237, 216, 52);
    
    palette->backlightFail = ccc3(125, 20, 36);
    palette->backlightSuccess = ccc3(76, 212, 176);
    
    palette->topPanelColor = ccc3(237, 216, 52);
    
    return palette;
}

Colors * Colors::truckBurgerPalette() {
    auto palette = new Colors();
    palette->background = ccc3(103, 151, 161);
    palette->blocks = ccc3(250, 191, 161);
    palette->ball = ccc3(227, 231, 177);
    palette->racquet = ccc3(227, 231, 177);
    
    palette->backlightFail = ccc3(250, 191, 161);
    palette->backlightSuccess = ccc3(76, 212, 176);
    
    palette->topPanelColor = ccc3(242, 248, 234);
    
    return palette;
}

int Colors::getColorsPalettesCount() {
    return 3;
}

Colors * Colors::colorsPaletteByIndex(int index) {
    index = index % getColorsPalettesCount();
    
    switch (index) {
        case 0: return flatAndHipPalette();
        case 1: return retroFlatPalette();
        case 2: return truckBurgerPalette();
            
        default:
            CCLOGERROR("Wrong palette index: %d", index);
            return NULL;
    }
    
}

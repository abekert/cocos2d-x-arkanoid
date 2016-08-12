//
//  LevelPresenter.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 26/07/16.
//
//

#pragma once

#include "cocos2d.h"
#include "Level.hpp"

class LevelPresenter : public CCNode
{
public:
    static void presentLevel(Level* level, CCNode* whereToPresent, int zOrder = 0);
    void presentLevelMoveStyle(Level* level, CCNode* whereToPresent, float duration = 0.5f, int zOrder = 0);
    void presentLevelLineByLine(Level* level, CCNode* whereToPresent, float duration = 0.5f, int zOrder = 0);

private:
    static CCPoint finalPositionForLevel();
    
    struct PresentRowParams {
        Level* level;
        int row;
    };
    void presentRow(CCNode *node, PresentRowParams *params);
};
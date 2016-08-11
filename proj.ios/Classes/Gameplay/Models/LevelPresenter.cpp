//
//  LevelPresenter.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 26/07/16.
//
//

#include "LevelPresenter.hpp"

CCPoint LevelPresenter::finalPositionForLevel() {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    return ccp(visibleSize.width/2 + origin.x, visibleSize.height);
}


void LevelPresenter::presentLevel(Level* level, CCNode* parent, int zOrder) {
    auto position = finalPositionForLevel();
    level->setPosition(position);
    level->removeFromParent();
    parent->addChild(level, zOrder);
}

void LevelPresenter::presentLevelMoveStyle(Level* level, CCNode* parent, float duration, int zOrder) {
    auto finalPosition = finalPositionForLevel();
    auto startPosition = ccp(finalPosition.x, finalPosition.y + level->levelHeigth);
    level->setPosition(startPosition);
    
    level->removeFromParent();
    parent->addChild(level, zOrder);
    
    auto move = CCMoveTo::create(duration, finalPosition);
    auto easedMove = CCEaseBackOut::create(move);

    level->runAction(easedMove);
}

void LevelPresenter::presentLevelLineByLine(Level* level, CCNode* parent, float duration, int zOrder) {
    presentLevel(level, parent);
    
    level->setVisible(false);
    
    int rowsNumber = level->getRowsNumber();
    float timePerRow = duration / rowsNumber;
    float delay = timePerRow;
    
    for (int row = 0; row < rowsNumber; row++) {
        auto wait = CCDelayTime::create(delay);
        PresentRowParams *params = new PresentRowParams;
        delay += timePerRow;
        
        params->level = level;
        params->row = row;
        auto enable = CCCallFuncND::create(this, callfuncND_selector(LevelPresenter::presentRow), (void*)params);
        level->runAction(CCSequence::createWithTwoActions(wait, enable));
    }
}

void LevelPresenter::presentRow(CCNode *node, PresentRowParams *paramsPointer) {
    PresentRowParams* params = static_cast<PresentRowParams*>(paramsPointer) ;
    
    params->level->setRowVisible(params->row);
    
    delete paramsPointer;
}
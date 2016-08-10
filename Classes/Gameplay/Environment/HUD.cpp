//
//  HUD.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 8/7/16.
//
//

#include "HUD.hpp"
//#include <stdlib.h>

static const int fadeTag = 1;

static const int halfBlackOpacity = 100;

USING_NS_CC;

bool HUD::init() {
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // Pause Button
    
    menu = CCMenu::create();
    menu->setPosition(CCPointZero);
    this->addChild(menu, 1);
    
    pauseButton = CCMenuItemImage::create("CloseNormal.png",
                                          "CloseSelected.png",
                                          this,
                                          menu_selector(HUD::pauseButtonCallback));
    CCSize buttonSize = pauseButton->getContentSize();
    CCPoint buttonPosition = ccp(origin.x + visibleSize.width - buttonSize.width / 2 , origin.y + visibleSize.height - buttonSize.height / 2);
    pauseButton->setPosition(buttonPosition);
    
    menu->addChild(pauseButton);
    
    // Scores
    
    scoresLabel = CCLabelTTF::create("Scores: 0", "Arial", 24);
//    auto scoreSize = scoresLabel->size
    // position the label on the center of the screen
    CCPoint scorePosition = ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - buttonSize.height / 2);

    scoresLabel->setPosition(scorePosition);
    
    // add the label as a child to this layer
    this->addChild(scoresLabel, 1);

    
    setupBlackScreen();
    
    paused = false;
    
    return true;
}

void HUD::setupBlackScreen() {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    black = ColorSprite::create(ccc4(0, 0, 0, 255), CCSize(1, 1));
    
    black->setScaleX(visibleSize.width);
    black->setScaleY(visibleSize.height);
    black->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    black->setOpacity(0);
    this->addChild(black);
}

void HUD::fadeBlackTo(int opacity, float duration) {
    auto fade = CCFadeTo::create(duration, opacity);
    fade->setTag(fadeTag);
    black->stopActionByTag(fadeTag);
    black->runAction(fade);
}


void HUD::pauseButtonCallback() {
    CCLOG("Pause Button Clicked");
    paused = !paused;
    
    auto opacity = paused ? halfBlackOpacity : 0;
    fadeBlackTo(opacity, 0.25f);

    if (paused) {
        presentExitButton();
    } else {
        exitButton->removeFromParent();
    }
    
    if (delegate) {
        if (paused) {
            delegate->gamePaused();
        } else {
            delegate->gameResumed();
        }
    }
}

void HUD::presentExitButton() {
    if (!exitButton) {
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

        CCLabelTTF* exitButtonLabel = CCLabelTTF::create("End Game", "Arial", 24);
        exitButtonLabel->setHorizontalAlignment(kCCTextAlignmentCenter);
        exitButtonLabel->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        exitButton = CCMenuItemLabel::create(exitButtonLabel, this, menu_selector(HUD::exitButtonCallback));
        auto labelSize = exitButton->getContentSize();
        exitButton->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        exitButton->retain();
    }
    
    menu->addChild(exitButton);
}

void HUD::exitButtonCallback() {
    if (delegate) {
        delegate->exitGame();
    }
}

void HUD::setScores(int scores) {
    this->scores = scores;
    
    if (scoresLabel) {
        char* numberstring;
        numberstring = new char[(((sizeof scores) * CHAR_BIT) + 2)/3 + 2];
        sprintf(numberstring, "Scores: %d", scores);
        scoresLabel->setString(numberstring);
    }
}
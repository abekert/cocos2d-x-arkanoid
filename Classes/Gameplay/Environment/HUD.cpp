//
//  HUD.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 8/7/16.
//
//

#include "HUD.hpp"

static const int fadeTag = 1;

static const int halfBlackOpacity = 120;

USING_NS_CC;

bool HUD::init() {
    if (!CCLayer::init()) {
        return false;
    }
    
    addTopPanel();
    
    setupBlackScreen();
    paused = false;
    
    return true;
}

void HUD::addTopPanel() {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    float itemHeight = visibleSize.height * 0.07f;
    normalFontSize = itemHeight * 0.8f;
    bigFontSize = 2 * normalFontSize;

    // Pause Button
    
    menu = CCMenu::create();
    menu->setPosition(CCPointZero);
    this->addChild(menu, 1);
    
    pauseButton = CCMenuItemImage::create("pause.png",
                                          "pause.png",
                                          this,
                                          menu_selector(HUD::pauseButtonCallback));
    CCSize buttonSize = pauseButton->getContentSize();
    float scale = itemHeight / buttonSize.height;
    pauseButton->setScale(scale);
    buttonSize = buttonSize * scale;
    
    CCPoint buttonPosition = ccp(origin.x + visibleSize.width - buttonSize.width / 2 , origin.y + visibleSize.height - buttonSize.height / 2);
    pauseButton->setPosition(buttonPosition);
    
    menu->addChild(pauseButton);
    
    auto fontName = "Arial";
    
    // Scores
    
    scoresLabel = CCLabelTTF::create("Scores: 0", fontName, normalFontSize);
    CCPoint scorePosition = ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - buttonSize.height / 2);
    
    scoresLabel->setPosition(scorePosition);
    
    this->addChild(scoresLabel, 1);
    
    // Lives
    
    livesLabel = CCLabelTTF::create("Lives: 0", fontName, normalFontSize);
    auto livesSize = livesLabel->getContentSize();
    livesLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
    CCPoint livesPosition = ccp(origin.x + livesSize.width * 0.6f, origin.y + visibleSize.height - buttonSize.height / 2);
    
    livesLabel->setPosition(livesPosition);
    
    this->addChild(livesLabel, 1);
}

void HUD::setTopColor(cocos2d::ccColor3B color) {
    scoresLabel->setColor(color);
    livesLabel->setColor(color);
    pauseButton->setColor(color);
}

HUD::~HUD() {
    if (exitButton) {
        exitButton->release();
    }
}

#pragma mark Black Screen

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

#pragma mark Pause Button

void HUD::pauseButtonCallback() {
    CCLOG("Pause Button Clicked");
    if (isPresenting) {
        return;
    }
    
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

#pragma mark Exit Button

void HUD::presentExitButton() {
    if (!exitButton) {
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

        CCLabelTTF* exitButtonLabel = CCLabelTTF::create("End Game", "Arial", normalFontSize);
        exitButtonLabel->setHorizontalAlignment(kCCTextAlignmentCenter);
        exitButtonLabel->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        exitButton = CCMenuItemLabel::create(exitButtonLabel, this, menu_selector(HUD::exitButtonCallback));
        auto labelSize = exitButton->getContentSize();
        exitButton->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.3f));
        exitButton->retain();
    }
    
    menu->addChild(exitButton);
}

void HUD::exitButtonCallback() {
    if (delegate) {
        delegate->exitGame();
    }
}

#pragma mark Top Bar

void HUD::setScores(int scores) {
    this->scores = scores;
    
    if (scoresLabel) {
        char numberstring[20];
        sprintf(numberstring, "Scores: %d", scores);
        scoresLabel->setString(numberstring);
    }
}

void HUD::setLives(int lives) {
    this->lives = lives;
    
    if (livesLabel) {
        char numberstring[20];
        sprintf(numberstring, "Lives: %d", lives);
        livesLabel->setString(numberstring);
    }
}

#pragma mark Level Presentation

void HUD::presentLevel(int levelNumber, float duration) {
    if (!bigLabel) {
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        
        bigLabel = CCLabelTTF::create("", "Arial", bigFontSize);
        bigLabel->setHorizontalAlignment(kCCTextAlignmentCenter);
        bigLabel->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        bigLabel->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        bigLabel->retain();
    }
    
    char string[20];
    sprintf(string, "Level %d", levelNumber);
    bigLabel->setString(string);
    bigLabel->setOpacity(0);
    this->addChild(bigLabel, 1);

    float durationAppear = duration * 0.25f;
    float durationShow = duration * 0.5f;
    float durationDisappear = duration * 0.25f;
    
    auto fadeInBlack = CCFadeTo::create(durationAppear, halfBlackOpacity);
    auto fadeInLabel = CCFadeIn::create(durationAppear);
    auto delay = CCDelayTime::create(durationShow);
    auto fadeOut = CCFadeTo::create(durationDisappear, 0);
    auto callback = CCCallFunc::create(this, callfunc_selector(HUD::endPresentation));
    
    if (black) {
        black->runAction(CCSequence::create(fadeInBlack, (CCAction *)delay->copy(), (CCAction *)fadeOut->copy(), NULL));
    }
    
    bigLabel->runAction(CCSequence::create(fadeInLabel, delay, fadeOut, callback, NULL));
    isPresenting = true;
}

void HUD::endPresentation() {
    CCLOG("Presentation end");
    bigLabel->removeFromParent();
    isPresenting = false;
}

void HUD::presentGameOver() {
    bigLabel->setString("Game Over");
    this->addChild(bigLabel, 1);
    
    auto fadeInLabel = CCFadeIn::create(0.25f);
    bigLabel->runAction(fadeInLabel);
    
    if (black) {
        auto fadeInBlack = CCFadeTo::create(3, halfBlackOpacity);
        black->runAction(fadeInBlack);
    }
    
    presentExitButton();
    isPresenting = true;
}

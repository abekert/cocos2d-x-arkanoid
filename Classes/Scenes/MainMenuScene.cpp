//
//  MainMenuScene.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 21/07/16.
//
//

#include "MainMenuScene.hpp"
#include "GameScene.hpp"

#include "../Gameplay/Environment/Colors.hpp"
#include "../Gameplay/Environment/SoundManager.hpp"

USING_NS_CC;

CCScene * MainMenuScene::scene()
{
    CCScene *scene = CCScene::create();
    MainMenuScene *layer = MainMenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainMenuScene::init()
{
    auto colorPalette = ColorsManager::sharedManager()->getCurrentColorsPalette();
    
    auto bg = colorPalette->background;
    if (!CCLayerColor::initWithColor(ccc4(bg.r, bg.g, bg.b, 255))) {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // Menu
    menu = CCMenu::create();
    menu->setPosition(CCPointZero);
    this->addChild(menu, 1);

    float itemHeight = visibleSize.height * 0.2f;

    // Arkanoid
    
    arkanoidLabel = CCLabelTTF::create("", "Arial", itemHeight * 0.6f);
    arkanoidLabel->setColor(colorPalette->blocks);
    CCPoint labelPosition = ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.75f);
    arkanoidLabel->setPosition(labelPosition);
    this->addChild(arkanoidLabel);
    
    // Credits
    
    creditsLabel = CCLabelTTF::create("Made with Cocos2d-x\nAlexander Bekert, 2016", "Arial", itemHeight * 0.12f);
    creditsLabel->setColor(colorPalette->blocks);
//    creditsLabel->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    creditsLabel->setAnchorPoint(ccp(0.5f, 0));
    labelPosition = ccp(origin.x + visibleSize.width / 2, origin.y);
    creditsLabel->setPosition(labelPosition);
    this->addChild(creditsLabel);

    // Start
    
    startButton = CCMenuItemImage::create("play.png",
                                          "play.png",
                                          this,
                                          menu_selector(MainMenuScene::startButtonCallback));
    CCSize buttonSize = startButton->getContentSize();
    float scale = itemHeight / buttonSize.height;
    startButton->setScale(scale);
    buttonSize = buttonSize * scale;

    CCPoint buttonPosition = ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    startButton->setPosition(buttonPosition);
    menu->addChild(startButton);
    
    addSoundButton();
    
    return true;
}

void MainMenuScene::onEnter() {
    CCLayerColor::onEnter();
    
    auto colorPalette = ColorsManager::sharedManager()->getCurrentColorsPalette();
    setColor(colorPalette->background);
    startButton->setColor(colorPalette->topPanelColor);
    soundButton->setColor(colorPalette->topPanelColor);
    arkanoidLabel->setColor(colorPalette->topPanelColor);
    creditsLabel->setColor(colorPalette->topPanelColor);
    
    animateLabelsAppear();
    
    CCLOG("On Enter");
}

#pragma mark Animation

void MainMenuScene::animateLabelsAppear() {
    creditsLabel->stopAllActions();
    arkanoidLabel->stopAllActions();
    arkanoidLabel->setString("");
    creditsLabel->setOpacity(0);
    
    auto wait = CCDelayTime::create(0.3f);
    char *a = "A";
    char *r = "R";
    char *k = "K";
    char *n = "N";
    char *o = "O";
    char *i = "I";
    char *d = "D";
    
    auto appendA = CCCallFuncND::create(this, callfuncND_selector(MainMenuScene::addLetterToLabel), a);
    auto appendR = CCCallFuncND::create(this, callfuncND_selector(MainMenuScene::addLetterToLabel), r);
    auto appendK = CCCallFuncND::create(this, callfuncND_selector(MainMenuScene::addLetterToLabel), k);
    auto appendN = CCCallFuncND::create(this, callfuncND_selector(MainMenuScene::addLetterToLabel), n);
    auto appendO = CCCallFuncND::create(this, callfuncND_selector(MainMenuScene::addLetterToLabel), o);
    auto appendI = CCCallFuncND::create(this, callfuncND_selector(MainMenuScene::addLetterToLabel), i);
    auto appendD = CCCallFuncND::create(this, callfuncND_selector(MainMenuScene::addLetterToLabel), d);
    auto showCredits = CCCallFunc::create(this, callfunc_selector(MainMenuScene::showCredits));
    
    auto sequence = CCSequence::create((CCDelayTime *)wait->copy(), (CCAction *)appendA->copy(), (CCDelayTime *)wait->copy(), appendR, (CCDelayTime *)wait->copy(), appendK, (CCDelayTime *)wait->copy(), appendA, (CCDelayTime *)wait->copy(), appendN, (CCDelayTime *)wait->copy(), appendO, (CCDelayTime *)wait->copy(), appendI, (CCDelayTime *)wait->copy(), appendD, (CCDelayTime *)wait->copy(), showCredits, NULL);
    
    arkanoidLabel->runAction(sequence);
}

void MainMenuScene::addLetterToLabel(CCNode* labelNode, void *letterPointer) {
    CCLabelTTF *label = (CCLabelTTF *)labelNode;
    char *letter = (char *)letterPointer;
    
    char result[100];
    strcpy(result, label->getString());
    strcat(result, letter);
    
    label->setString(result);
}

void MainMenuScene::showCredits() {
    if (!creditsLabel->getParent()) {
        this->addChild(creditsLabel);
    }
    
    creditsLabel->setOpacity(0);
    auto fadeIn = CCFadeIn::create(1);
    creditsLabel->runAction(fadeIn);
}

#pragma mark Game Start

void MainMenuScene::startButtonCallback(CCObject* sender) {
    CCLOG("Start Button Pressed");
    toGameScene();
}

void MainMenuScene::toGameScene() {
    auto scene = GameScene::scene();
    auto transition = CCTransitionMoveInR::create(0.1f, scene);
//    auto transition = CCTransitionFlipX::create(0.1f, scene);
//    CCDirector::sharedDirector()
//    CCDirector::sharedDirector()->replaceScene(transition);
    CCDirector::sharedDirector()->pushScene(transition);
}

#pragma mark Sound

#define soundButtonImage(soundEnabled) (soundEnabled ? "sound-on.png" : "sound-off.png")

void MainMenuScene::addSoundButton() {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    bool soundEnabled = SoundManager::sharedManager()->getSoundEnabled();

    soundButton = CCMenuItemImage::create(soundButtonImage(soundEnabled),
                                          soundButtonImage(soundEnabled),
                                          this,
                                          menu_selector(MainMenuScene::soundButtonCallback));
    CCSize buttonSize = soundButton->getContentSize();
    float itemHeight = visibleSize.height * 0.1f;
    float scale = itemHeight / buttonSize.height;
    soundButton->setScale(scale);
    buttonSize = buttonSize * scale;
    
    CCPoint buttonPosition = ccp(origin.x + visibleSize.width - buttonSize.width *0.75f, origin.y + buttonSize.height * 0.75f);
    soundButton->setPosition(buttonPosition);
    menu->addChild(soundButton);
}

void MainMenuScene::soundButtonCallback(CCObject* sender) {
    bool soundEnabled = SoundManager::sharedManager()->getSoundEnabled();
    soundEnabled = !soundEnabled;
    SoundManager::sharedManager()->setSoundEnabled(soundEnabled);
    
    if (soundEnabled) {
        SoundManager::sharedManager()->playEnableSound();
    }
    
    soundButton->initWithNormalImage(soundButtonImage(soundEnabled),
                                     soundButtonImage(soundEnabled),
                                     soundButtonImage(soundEnabled),
                                     this,
                                     menu_selector(MainMenuScene::soundButtonCallback));
}
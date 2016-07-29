//
//  MainMenuScene.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 21/07/16.
//
//

#include "MainMenuScene.hpp"
#include "GameScene.hpp"

#include "../Gameplay/Models/Block.hpp"

USING_NS_CC;

CCScene* MainMenuScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainMenuScene *layer = MainMenuScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // create menu, it's an autorelease object
    CCMenu* menu = CCMenu::create();
    menu->setPosition(CCPointZero);
    this->addChild(menu, 1);

    CCLabelTTF* startLabelTTF = CCLabelTTF::create("Start", "Arial", 24);

    CCMenuItemLabel *startLabel = CCMenuItemLabel::create(startLabelTTF, this, menu_selector(MainMenuScene::startButtonCallback));
    startLabelTTF->setHorizontalAlignment(kCCTextAlignmentCenter);
    startLabelTTF->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    auto labelSize = startLabel->getContentSize();
    startLabel->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    menu->addChild(startLabel);
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(MainMenuScene::menuCloseCallback));
    
    pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));
    
    menu->addChild(pCloseItem);
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(pLabel, 1);
    
//    // add "HelloWorld" splash screen"
//    CCSprite* pSprite = CCSprite::create("HelloWorld.png");
//    // position the sprite on the center of the screen
//    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//    // add the sprite as a child to this layer
//    this->addChild(pSprite, 0);
    
//    auto block = Block::create(500, 500);
//    block->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//    this->addChild(block, 10);
    
    return true;
}


void MainMenuScene::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void MainMenuScene::startButtonCallback(CCObject* sender)
{
    CCLOG("Start Button Pressed");
    toGameScene();
}

void MainMenuScene::toGameScene()
{
    //get the game scene and run it.
    auto scene = GameScene::scene();
    auto transition = CCTransitionFlipX::create(0.2f, scene);
//    CCDirector::sharedDirector()
    CCDirector::sharedDirector()->replaceScene(transition);
}


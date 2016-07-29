//
//  GameScene.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 22/07/16.
//
//

#include "GameScene.hpp"
#include "../Gameplay/Models/Level.hpp"
#include "../Gameplay/Models/LevelPresenter.hpp"

USING_NS_CC;

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
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
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(GameScene::menuCloseCallback));
    
    pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));
    
    menu->addChild(pCloseItem);
    
    CCMenuItemImage *pCloseItem2 = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(GameScene::delayHideCallback));
    
    pCloseItem2->setPosition(ccp(origin.x + visibleSize.width - 3*pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));
    
    menu->addChild(pCloseItem2);
    
    CCMenuItemImage *pCloseItem3 = CCMenuItemImage::create(
                                                           "CloseNormal.png",
                                                           "CloseSelected.png",
                                                           this,
                                                           menu_selector(GameScene::testAction));
    
    pCloseItem3->setPosition(ccp(origin.x + visibleSize.width - 5*pCloseItem->getContentSize().width/2 ,
                                 origin.y + pCloseItem->getContentSize().height/2));
    
    menu->addChild(pCloseItem3);


    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("It is a game scene", "Arial", 24);
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(pLabel, 1);
    
    // add "HelloWorld" splash screen"
    
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");
    
    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    Level::setupDefaults();
    level = Level::createSampleLevel(8, 8);
//    LevelPresenter::presentLevelMoveStyle(level, this, 1, 10);
    auto presenter = new LevelPresenter;
    presenter->presentLevelLineByLine(level, this, 2, 10);

    return true;
}


void GameScene::menuCloseCallback(CCObject* pSender)
{
    auto block = level->getBlock(1, 2);
    block->setVisible(!block->isVisible());
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
//#else
//    CCDirector::sharedDirector()->end();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
//#endif
}

void GameScene::delayHideCallback(CCObject* pSender)
{
    auto block = level->getBlock(1, 2);
    block->setVisible(1, false);
}

void GameScene::testAction(CCObject* pSender)
{
    auto block = level->getBlock(1, 2);
    auto rotate = CCRotateBy::create(1, 180);
//    auto move = CCMoveBy::create(1, ccp(0, 5));
    block->runAction(rotate);
    CCLog("rotate");
//    this->runAction(CCSequence::createWithTwoActions(wait, enable));
}





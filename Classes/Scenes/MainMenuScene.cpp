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
#include "../Gameplay/Environment/Backlight.hpp"

USING_NS_CC;

#define SCALE_RATIO 32.0

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
    pCloseItem = CCMenuItemImage::create(
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
//    
//    auto block = Block::create(500, 500);
//    block->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//    this->addChild(block, 10);
    
    b2Vec2 gravity = b2Vec2(0.0f,0); // Vector of acceleration ( the mark "-" is for down direction, because the y axis is upwards )
    
    world = new b2World(gravity); // Create world with vector of acceleration

    // Create a ball Sprite
    ball = CCSprite::create("CloseNormal.png");
    
    // Set the position in the center of the screen
    ball->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // This block is the most important to apply body in Box2D
    
    //---------------------physics body in BOX2D--------------------
    
    bodyShape.m_radius = ball->getContentSize().width / 2 / SCALE_RATIO; // The radius of body
    
    //fixtureDef
    fixtureDef.density=10;
    fixtureDef.friction=0;
    fixtureDef.restitution=1;
    fixtureDef.shape=&bodyShape; // point to bodyShape
    
    //bodyDef
    bodyDef.type = b2_dynamicBody; // Dynamic collision
    bodyDef.userData = ball; // Attach to Sprite ball
    
    // Set position, and remember to convert the unit
    bodyDef.position.Set(ball->getPosition().x/SCALE_RATIO,ball->getPosition().y/SCALE_RATIO);
    
    //ballBody
    ballBody = world->CreateBody(&bodyDef); // Create Body
    ballBody->CreateFixture(&fixtureDef); // Create static features
    ballBody->SetGravityScale(1); // Set the ratio for acceleration, more higher more faster to fall
    
    //-----------------------------------------------------------
    
    // Place the ball into layer of Scene
    this->addChild(ball, 0);
    
    scheduleUpdate(); // Update again the scene by time
    setupLoose();

    addWall(visibleSize.width, visibleSize.height);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

    world->SetContactListener(this);
    
    return true;
}

void MainMenuScene::setupLoose() {
    backlight = Backlight::addTo(this);
    backlight->sprite->setColor(ccc3(255, 0, 0));
    backlight->sprite->setOpacity(0);
}

void MainMenuScene::update(float dt){
    int positionIterations = 10;  // Location
    int velocityIterations = 10; // Velocity
    
    deltaTime = dt; // Time step
    
    // Simulate the physical movement by time, study here http://www.box2d.org/manual.html and here  http://www.iforce2d.net/b2dtut/worlds
    
    // Each Step happens in dt seconds , this dt in file AppDelegate.cpp is defined by command director->setAnimationInterval(1.0 / 60); you try replacing 1/60 = 1/1 , it falls very slow
    
    world->Step(dt, velocityIterations, positionIterations);
    
    // Navigate all bodies of world
    for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext())
        // Consider which body is attached to Sprite
        if (body->GetUserData())
        {
            
            // return the ball sprite
            CCSprite *sprite = (CCSprite *) body->GetUserData();
            // Set the position again for this Sprite follow the position of body ( body is gradually falling by time), remember to multiply with RATIO to convert into pixel
            sprite->setPosition(ccp(body->GetPosition().x * SCALE_RATIO,body->GetPosition().y * SCALE_RATIO));
            // Set the rotation ability
            sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
            
        }
    world->ClearForces(); // Clear forces of Body
    world->DrawDebugData();  // Draw shapes to check collision in debug mode
}

/// Create the Wall edge around the screen for the ball to collide with.
void MainMenuScene::addWall(float w,float h) {
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0,0);
    
    b2Body *groundBody = world->CreateBody(&groundBodyDef);
    b2EdgeShape groundEdge;
    b2FixtureDef boxShapeDef;
    boxShapeDef.shape = &groundEdge;
    
    //wall definitions
    backlight->setTag(10);
    groundBody->SetUserData((void *)backlight);
    groundEdge.Set(b2Vec2(0,0), b2Vec2(w/SCALE_RATIO, 0));
    groundBody->CreateFixture(&boxShapeDef);
    
    groundBody = world->CreateBody(&groundBodyDef);
    
    groundEdge.Set(b2Vec2(0,0), b2Vec2(0,w/SCALE_RATIO));
    groundBody->CreateFixture(&boxShapeDef);
    
    groundEdge.Set(b2Vec2(0, h/SCALE_RATIO), b2Vec2(w/SCALE_RATIO, h/SCALE_RATIO));
    groundBody->CreateFixture(&boxShapeDef);
    
    groundEdge.Set(b2Vec2(w/SCALE_RATIO, h/SCALE_RATIO), b2Vec2(w/SCALE_RATIO, 0));
    groundBody->CreateFixture(&boxShapeDef);
}

void MainMenuScene::menuCloseCallback(CCObject* pSender)
{
    float speed = 50;
    b2Vec2 vector = ballBody->GetLinearVelocity();
    vector.Normalize();
    vector = speed * vector;
    ballBody->SetLinearVelocity(vector);
    
    backlight->blink(1);

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
//#else
//    CCDirector::sharedDirector()->end();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
//#endif
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
    auto transition = CCTransitionMoveInR::create(0.1f, scene);
//    auto transition = CCTransitionFlipX::create(0.1f, scene);
//    CCDirector::sharedDirector()
//    CCDirector::sharedDirector()->replaceScene(transition);
    CCDirector::sharedDirector()->pushScene(transition);
}

bool MainMenuScene::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    CCLOG("touch!");
//    ballBody->ApplyForceToCenter(b2Vec2(0, 50000));
//    float force = 10000;
    
    auto touchPosition = touch->getLocationInView();
    
    auto nodeSpaceLocation = pCloseItem->getParent()->convertToNodeSpace(touchPosition);
//    cc->rectContainsPoint
    
    
//    CCRectContainsPoint();
//    pCloseItem->rect()
    
    if (pCloseItem->boundingBox().containsPoint(nodeSpaceLocation)) {
        CCLOG("button");
        return false;
    }
    
    auto vector = ball->getPosition() - touch->getLocationInView();
    vector.y = -vector.y;
    float force = 10 * vector.getLengthSq();
    vector = vector * force;
    ballBody->SetLinearVelocity(b2Vec2(0, 0));
    ballBody->ApplyForceToCenter(b2Vec2(vector.x, vector.y));
    CCLOG("Force: %0.2lf (x: %0.2lf, y: %0.2lf)", force, vector.x, vector.y);

    return false;
}


void MainMenuScene::BeginContact(b2Contact* contact)
{
    // We need to copy out the data because the b2Contact passed in
    // is reused.
    CCNode * a = (CCNode *)contact->GetFixtureA()->GetBody()->GetUserData();
    CCNode * b = (CCNode *)contact->GetFixtureB()->GetBody()->GetUserData();

    if (a) {
        CCLOG("a: %d", a->getTag());
    }
    if (b) {
        CCLOG("b: %d", b->getTag());
    }
    
    if (a == backlight) {
        backlight->blink(1);
    }
    
    if (b == backlight) {
        backlight->blink(1);
    }

//    auto aData = (CCNode *)a->GetUserData();
//    aData->getTag()
//    if (a->GetUserData())
//    {
//        // return the ball sprite
//        CCSprite *sprite = (CCSprite *) a->GetUserData();
//        
//        if (sprite != NULL) {
//            CCLOG("a: sprite not null");
//        }
//        // Set the position again for this Sprite follow the position of body ( body is gradually falling by time), remember to multiply with RATIO to convert into pixel
//    }
//    
//    if (b->GetUserData())
//    {
//        // return the ball sprite
//        CCSprite *sprite = (CCSprite *) b->GetUserData();
//        if (sprite != NULL) {
//            CCLOG("a: sprite not null");
//        }
//        // Set the position again for this Sprite follow the position of body ( body is gradually falling by time), remember to multiply with RATIO to convert into pixel
//    }

    CCLOG("contact");
//    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
//    _contacts.push_back(myContact);
}



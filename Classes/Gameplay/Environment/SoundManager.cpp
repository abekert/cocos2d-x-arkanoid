//
//  SoundManager.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 12/08/16.
//
//

#include "SoundManager.hpp"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

USING_NS_CC;

static SoundManager *_sharedManager = NULL;

SoundManager * SoundManager::sharedManager() {
    if (!_sharedManager)
    {
        _sharedManager = new SoundManager();
    }
    
    return _sharedManager;
}

SoundManager::SoundManager() {
    soundEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey("Sound", true);
}

void SoundManager::setSoundEnabled(bool enabled) {
    soundEnabled = enabled;
    CCUserDefault::sharedUserDefault()->setBoolForKey("Sound", enabled);
}

void SoundManager::playSound(const char *name) {
    if (!soundEnabled) {
        return;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(name);
}

void SoundManager::playBallTouchedBlock() {
    playSound("kick.wav");
}

void SoundManager::playBallTouchedRaquet() {
    playSound("raquet.wav");
}

void SoundManager::playBallTouchedBottom() {
    playSound("fail.wav");
}

void SoundManager::playGameStart() {
    playSound("game-start.wav");
}

void SoundManager::playGameOver() {
    playSound("game-over.wav");
}

void SoundManager::playLevelComplete() {
    playSound("success.wav");
}

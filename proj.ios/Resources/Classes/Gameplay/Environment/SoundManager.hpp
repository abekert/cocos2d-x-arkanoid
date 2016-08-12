//
//  SoundManager.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 12/08/16.
//
//

#pragma once

class SoundManager {
public:
    static SoundManager * sharedManager();
    
    bool getSoundEnabled() { return soundEnabled; }
    void setSoundEnabled(bool enabled = true);
    
    void playBallTouchedBlock();
    void playBallTouchedRaquet();
    void playBallTouchedBottom();

    void playGameStart();
    void playGameOver();
    void playLevelComplete();

    void playEnableSound() { playGameStart(); }

private:
    SoundManager();

    void playSound(const char *name);
    bool soundEnabled;
};
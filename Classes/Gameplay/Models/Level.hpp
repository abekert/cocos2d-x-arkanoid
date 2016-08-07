//
//  Level.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 24/07/16.
//
//

#pragma once

#include "cocos2d.h"
#include "Block.hpp"

class LevelCompleteDelegate {
public:
    virtual void levelComplete() { CCLOG("Level Complete"); }
};

class Level : public cocos2d::CCNode, public BlockStatesDelegate
{
public:
    static float levelWidth;
    static float levelHeigth;
    static float padding;
    
    static void setupDefaults();
    static Level* createSampleLevel(int columnsNumber, int rowsNumber);

    Block* getBlock(int row, int column);
    void setBlock(Block *block, int row, int column);
    int getColumnsNumber() { return columnsNumber; };
    int getRowsNumber() { return rowsNumber; };    
    
    void setVisible(bool visible = true);
    void setRowVisible(int row, bool visible = true);
    
    void blockChangedState(Block *block);

    LevelCompleteDelegate *delegate;
    
private:
    std::vector<Block*> blocks;
    int blocksLength = 0;
    int columnsNumber = 0;
    int rowsNumber = 0;
    int blocksLeft = 0;
    
    CREATE_FUNC(Level);
    
    void initBlocksArray(int rowsNumber, int columnsNumber);
    
    struct BlockPresentationParams {
        float delay;
        int index;
    };
    void presentBlock(BlockPresentationParams* blockParams);

};
//
//  Level.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 24/07/16.
//
//

#include "Level.hpp"

float Level::levelWidth;
float Level::levelHeigth;
float Level::padding;

void Level::setupDefaults() {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    levelWidth = visibleSize.width * 0.90f;
    levelHeigth = visibleSize.height * 0.5f;
    padding = 4;
}

Level* Level::createSampleLevel(int columnsNumber, int rowsNumber) {
    auto level = Level::create();
    level->columnsNumber = columnsNumber;
    level->rowsNumber = rowsNumber;
    CCLog("Create sample level %dx%d", rowsNumber, columnsNumber);

    float blockWidth = ((levelWidth + padding) / columnsNumber) - padding;
    float blocksHeight = ((levelHeigth + padding) / rowsNumber) - padding;
    CCLog("blockWidth: %.2f, blocksHeight: %.2f", blockWidth, blocksHeight);

    float startX = -levelWidth / 2 + blockWidth / 2;
    float x = startX;
    float y = -(blocksHeight / 2) - padding;
    
    level->initBlocksArray(rowsNumber, columnsNumber);
    for (int row = 0; row < rowsNumber; row++) {
        x = startX;
        for (int column = 0; column < columnsNumber; column++) {
            auto block = Block::create(blockWidth, blocksHeight);
            block->setPosition(ccp(x, y));
            level->setBlock(block, row, column);
            level->addChild(block);
            x += blockWidth + padding;
        }
        y -= blocksHeight + padding;
    }
    
    return level;
}

// MARK: Blocks Array

void Level::initBlocksArray(int rowsNumber, int columnsNumber) {
    this->columnsNumber = columnsNumber;
    this->rowsNumber = rowsNumber;
    blocksLength = columnsNumber * rowsNumber;
//    blocks = new Block[blocksLength];
    blocks = *new std::vector<Block*>(blocksLength);
}

Block* Level::getBlock(int row, int column) {
    int index = row * columnsNumber + column;
    if (index < 0 || index >= blocksLength) {
        CCLOGERROR("Wrong block index: %d. Must be in range [0..%d]", index, blocksLength - 1);
        return NULL;
    }
//    return &blocks[index];
    return blocks.at(index);
}

void Level::setBlock(Block *block, int row, int column) {
    int index = row * columnsNumber + column;
    if (index < 0 || index >= blocksLength) {
        CCLOGERROR("Wrong block index: %d. Must be in range [0..%d]", index, blocksLength - 1);
        return;
    }
    
//    Block* existingBlock = &blocks[index];
    Block* existingBlock = blocks.at(index);
    if (existingBlock != NULL) {
        existingBlock->release();
    }
    
    block->retain();
    blocks[index] = block;
}

// MARK: Presentation

void Level::setVisible(bool visible) {
    for (int i = 0; i < blocksLength; i++) {
        auto block = blocks[i];
        block->setVisible(visible);
    }
}

void Level::setRowVisible(int row, bool visible) {
    for (int column = 0; column < columnsNumber; column++) {
        auto block = getBlock(row, column);
        if (block == NULL) {
            continue;
        }
        block->setVisible(visible);
    }
}

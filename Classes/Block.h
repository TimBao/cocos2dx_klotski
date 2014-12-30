#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "cocos2d.h"

enum BlockType
{
    BT_Invalid = 0,
    BT_Single,
    BT_Vertical,
    BT_Horizon,
    BT_Square
};

class KlotskiSolver;

class Block
{
public:
    Block(BlockType type, int x, int y);
    ~Block();

    bool attach(cocos2d::CCNode* parent);
    void detach(cocos2d::CCNode* parent);

    BlockType getType() { return m_type; }
    cocos2d::CCRect getBoundingBox() { return m_sprite->boundingBox(); }
    int getPositionX() { return m_x; }
    int getPositionY() { return m_y; }
    int getPositionBottom();
    int getPositionRight();

    void select();
    void unselect();
    void moveTo(int x, int y);

    void mask(KlotskiSolver* mask);
private:
    cocos2d::CCSprite* m_sprite;
    cocos2d::CCSpriteFrame* m_normalFrame;
    cocos2d::CCSpriteFrame* m_selectedFrame;
    BlockType m_type;
    int m_y;
    int m_x;
};

#endif //__BLOCK_H__
#ifndef __KLOTSKIPUZZLE_H__
#define __KLOTSKIPUZZLE_H__

#include "cocos2d.h"
#include <string>

#define  BLOCK_NUM 10

enum MoveDirection
{
    MD_LEFT = 0,
    MD_RIGHT,
    MD_TOP,
    MD_DOWN
};

class KlotskiPuzzleListener
{
public:
    virtual void moveToSuccess(int step) = 0;
    virtual void solvedSuccess() = 0;
    virtual void tooMuchStep() = 0;
};

class Block;
class KlotskiSolver;

class KlotskiPuzzle
{
public:
    explicit KlotskiPuzzle(KlotskiPuzzleListener* listener);
    ~KlotskiPuzzle();

    void init();
    bool isSolved();

    void startGame(const char* board);

    bool attach(cocos2d::CCNode* parent);
    void detach(cocos2d::CCNode* parent);

    void touched(cocos2d::CCPoint point);
    void moveTo(cocos2d::CCPoint point, MoveDirection dir);

    KlotskiSolver toMask();
    std::string serilize();
    int getCurrentStep() { return m_step; }
    void setCurrentStep(int step) { m_step = step; }

private:
    Block* m_blocks[BLOCK_NUM];
    int m_step;
    KlotskiPuzzleListener* m_listener;
};

#endif //__KLOTSKIPUZZLE_H__
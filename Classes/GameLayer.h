#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"
#include "KlotskiPuzzle.h"
#include <string>

class PopView : public cocos2d::CCLayer
{
public:
    PopView();
    ~PopView();
    bool init();
    CREATE_FUNC(PopView);

    void menuClick(cocos2d::CCObject* object);

    int m_currentLevel;
    cocos2d::CCNode* m_parent;
};

class KlotskiPuzzle;

class GameLayer : public cocos2d::CCLayer, KlotskiPuzzleListener
{
public:
    GameLayer();
    ~GameLayer();

    CREATE_FUNC(GameLayer);
    virtual bool init();

    void startGame(int level);
    void resumeGame();

    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    // from KlotskiPuzzleListener
    virtual void moveToSuccess(int step);
    virtual void solvedSuccess();
    virtual void tooMuchStep() {}

    void menuClick(cocos2d::CCObject* object);
    virtual void keyBackClicked(void);
private:
    cocos2d::CCSprite* m_frame;
    KlotskiPuzzle* m_puzzle;
    cocos2d::CCPoint m_touchPoint;
    cocos2d::CCLabelBMFont* m_current;
    cocos2d::CCLabelBMFont* m_currentValue;
    int m_currentLevel;
};

#endif //__GAMELAYER_H__
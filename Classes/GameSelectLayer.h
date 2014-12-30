#ifndef __GAMESELECTLAYER_H__
#define __GAMESELECTLAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

class LsTouchEvent;

/*
 * Used to define the sprite which will be touched.
 */
class LsTouch: public cocos2d::CCNode
{
public:
    LsTouch();
    ~LsTouch();
    CREATE_FUNC(LsTouch);
    virtual bool init();

    void setDisplay(cocos2d::CCSprite* dis);
    cocos2d::CCSprite* getDisplay();

    void setEventId(int eventId);
    int getEventId();

    bool selfCheck(cocos2d::CCTouch* ccTouch, LsTouchEvent* lsTe);

private:
    bool containsCCTouchPoint(cocos2d::CCTouch* ccTouch);
    bool isParentAllVisible(LsTouchEvent* lsTe);

    int m_iEventId;
};

class LsTouchEvent
{
public:
    LsTouchEvent();
    ~LsTouchEvent();

    void addLsTouch(LsTouch* touch, int eventId);

    void removeLsTouch(LsTouch* touch);

    bool sendTouchMessage(cocos2d::CCTouch* ccTouch, int type);

    LsTouch* getPriorityTouch(LsTouch* a, LsTouch* b);

    virtual void touchEventAction(LsTouch* touch, int type) = 0;
private:
    cocos2d::CCArray* m_pLsTouches;
};

class GameSelectLayer : public cocos2d::CCLayer, public LsTouchEvent
{
public:

    GameSelectLayer();
    ~GameSelectLayer();

    bool init();
    CREATE_FUNC(GameSelectLayer);

    virtual void touchEventAction(LsTouch* touch, int type);

    virtual void onEnter();
    virtual void onExit();

    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    virtual void keyBackClicked(void);
private:
    void adjustScrollView(float offset);
    cocos2d::CCLayer* getContainLayer();
    cocos2d::CCNode* getSpriteByLevel(int level);
    void setCurPageBall();
private:
    cocos2d::extension::CCScrollView* m_scrollView;
    cocos2d::CCPoint m_touchPoint;
    cocos2d::CCPoint m_touchOffset;

    int m_curPage;
    int m_pageCount;
};

#endif
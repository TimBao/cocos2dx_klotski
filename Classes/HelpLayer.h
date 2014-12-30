#ifndef __HELPLAYER_H__
#define __HELPLAYER_H__

#include "cocos2d.h"

class HelpLayer :  public cocos2d::CCLayer
{
public:
    HelpLayer();
    ~HelpLayer();

    bool init();
    CREATE_FUNC(HelpLayer);

    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
private:

};

#endif //__HELPLAYER_H__
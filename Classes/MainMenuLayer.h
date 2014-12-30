#ifndef __MAINMENULAYER_H__
#define __MAINMENULAYER_H__

#include "cocos2d.h"

class MainMenuLayer : public cocos2d::CCLayer
{
public:
    MainMenuLayer();
    ~MainMenuLayer();

    bool init();
    CREATE_FUNC(MainMenuLayer);

    void menuClick(cocos2d::CCObject* object);
    virtual void keyBackClicked(void);
private:

};

#endif //__MAINMENULAYER_H__
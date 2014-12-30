#ifndef __BACKGROUNDLAYER_H__
#define __BACKGROUNDLAYER_H__

#include "cocos2d.h"

class BackGroundLayer : public cocos2d::CCLayer
{
public:
    BackGroundLayer();
    ~BackGroundLayer();

    CREATE_FUNC(BackGroundLayer);
    virtual bool init();

private:

};

#endif //__BACKGROUNDLAYER_H__
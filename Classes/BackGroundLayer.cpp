#include "BackgroundLayer.h"
#include "WapsAd.h"

USING_NS_CC;

BackGroundLayer::BackGroundLayer()
{
}

BackGroundLayer::~BackGroundLayer()
{
}

bool BackGroundLayer::init()
{
    bool ret = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        CCSprite* background = CCSprite::create("bg.png");
        CC_ASSERT(background != NULL);
        background->setAnchorPoint(ccp(0, 0));
        background->setPosition(ccp(0,0));
        this->addChild(background);
        ret = true;

        WapsAd::showAd(0);
    } while (0);
    return ret;
}

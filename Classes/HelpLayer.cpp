#include "HelpLayer.h"
#include "GameScene.h"

USING_NS_CC;

HelpLayer::HelpLayer()
{
}

HelpLayer::~HelpLayer()
{
}

bool HelpLayer::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(!CCLayer::init());

        CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

        //help image
        CCSprite* background = CCSprite::create("help.png");
        CC_ASSERT(background != NULL);
        background->setAnchorPoint(ccp(0, 0));
        background->setPosition(ccp(0,0));
        this->addChild(background);

        this->setTouchEnabled(true);

        bRet = true;
    } while (0);
    return bRet;
}

void HelpLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool HelpLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void HelpLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    GameScene::instance()->hideHelp();
}
#include "MainMenuLayer.h"
#include "GameScene.h"
#include "WapsAd.h"

USING_NS_CC;

MainMenuLayer::MainMenuLayer()
{
}

MainMenuLayer::~MainMenuLayer()
{
}

bool MainMenuLayer::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(!CCLayer::init());

        CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

        // new game button
        CCMenuItemImage* btn_start = CCMenuItemImage::create("btn1.png", "btn1.png", "btn1.png");
        btn_start->setTarget(this, menu_selector(MainMenuLayer::menuClick));
        btn_start->setTag(0);

        // resume game button
        CCMenuItemImage* btn_resume = CCMenuItemImage::create("btn2.png", "btn2.png", "btn2_n.png");
        btn_resume->setTarget(this, menu_selector(MainMenuLayer::menuClick));
        btn_resume->setTag(1);

        std::string saveBoard = CCUserDefault::sharedUserDefault()->getStringForKey("save", "");
        if (saveBoard.empty())
        {
            btn_resume->setEnabled(false);
        }

        //help button
        CCMenuItemImage* btn_help = CCMenuItemImage::create("btn3.png", "btn3.png", "btn3.png");
        btn_help->setTarget(this, menu_selector(MainMenuLayer::menuClick));
        btn_help->setTag(2);

        CCMenu* menu = CCMenu::create(btn_start, btn_resume, btn_help, NULL);
        menu->setPosition(ccp(winSize.width/2 , winSize.height/2 - 160 - btn_resume->getContentSize().height*2));
        menu->alignItemsVerticallyWithPadding(80);
        this->addChild(menu);

        this->setKeypadEnabled(true);

        WapsAd::showAd(0);

        bRet = true;
    } while (0);
    return bRet;
}

void MainMenuLayer::menuClick(CCObject* object)
{
    CCSprite* sprite = static_cast<CCSprite*>(object);
    if (sprite->getTag() == 0)
    {
        GameScene::instance()->startNewGame();
    }
    else if (sprite->getTag() == 1)
    {
        GameScene::instance()->resumeGame();
    }
    else if (sprite->getTag() == 2)
    {
        GameScene::instance()->showHelp();
    }
}

void MainMenuLayer::keyBackClicked(void)
{
    CCLOG("back key click MainMenuLayer");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo showAd;

    bool isHave = JniHelper::getStaticMethodInfo(showAd,"com/tim/game/klotski/klotski","showAdStatic", "(I)V");

    if (!isHave)
    {
        CCLog("jni:showAdStatic is null");
    } else
    {
        showAd.env->CallStaticVoidMethod(showAd.classID, showAd.methodID, 13);
    }
#endif
}
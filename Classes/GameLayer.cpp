#include "GameLayer.h"
#include "KlotskiPuzzle.h"
#include <math.h>
#include "WapsAd.h"
#include "GameScene.h"
#include <string>

USING_NS_CC;

PopView::PopView()
    : m_currentLevel(0)
    , m_parent(NULL)
{
}

PopView::~PopView()
{
}

bool PopView::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();

        CCSprite* pop_bg = CCSprite::create("pop_bg.png");
        pop_bg->setPosition(ccp(winSize.width/2, winSize.height/2));
        this->addChild(pop_bg);

        CCLabelBMFont* label = CCLabelBMFont::create("恭喜过关！！！", "hrd.fnt");
        label->setPosition(ccp(pop_bg->getContentSize().width/2, pop_bg->getContentSize().height/2 + 50));
        pop_bg->addChild(label);

        CCMenuItemImage* btn_ok = CCMenuItemImage::create("btn_menu.png", "btn_menu.png", "btn_menu.png");
        btn_ok->setTarget(this, menu_selector(PopView::menuClick));

        CCLabelBMFont* label_ok = CCLabelBMFont::create("继续", "hrd.fnt");
        CCSize btnSize = btn_ok->getContentSize();
        label_ok->setPosition(ccp(btnSize.width/2, btnSize.height/2));
        btn_ok->addChild(label_ok);

        CCMenu* menu = CCMenu::create(btn_ok, NULL);
        menu->setPosition(ccp(pop_bg->getContentSize().width/2, pop_bg->getContentSize().height/2 - btnSize.height));
        pop_bg->addChild(menu);

        bRet = true;
    } while (0);
    return bRet;
}

void PopView::menuClick(cocos2d::CCObject* object)
{
    int nextLevel = m_currentLevel+1;
    if (nextLevel > 15)
    {
        nextLevel = 2;
    }
    m_parent->removeChild(this);
    GameScene::instance()->startNextGame(nextLevel);
}

GameLayer::GameLayer()
    : m_frame(NULL)
    , m_current(NULL)
    , m_currentValue(NULL)
    , m_puzzle(NULL)
    , m_currentLevel(0)
{
}

GameLayer::~GameLayer()
{
    CC_SAFE_RELEASE(m_frame);
}

bool GameLayer::init()
{
    bool ret = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();

        CCSprite* background = CCSprite::create("bg2.png");
        CC_ASSERT(background != NULL);
        background->setAnchorPoint(ccp(0, 0));
        background->setPosition(ccp(0,0));
        this->addChild(background, -1);

        m_frame = CCSprite::create("kuang.png");
        CC_ASSERT(m_frame != NULL);
        m_frame->setPosition(ccp(winSize.width / 2, winSize.height/2 - 50));
        m_frame->retain();
        this->addChild(m_frame);

        // current
        m_current = CCLabelBMFont::create("当前: ", "hrd.fnt");
        m_current->setAnchorPoint(CCPointZero);
        m_current->setPosition(ccp(origin.x + 40, winSize.height - 430));
        m_frame->addChild(m_current);

        m_currentValue = CCLabelBMFont::create("0", "hrd.fnt");
        m_currentValue->setAnchorPoint(CCPointZero);
        m_currentValue->setPosition(ccp(origin.x + m_current->getContentSize().width + 40, winSize.height - 435));
        m_frame->addChild(m_currentValue);

        //back key
        CCMenuItemImage* btn_start = CCMenuItemImage::create("btn_menu.png", "btn_menu.png", "btn_menu.png");
        btn_start->setTarget(this, menu_selector(GameLayer::menuClick));

        CCLabelBMFont* backkey = CCLabelBMFont::create("back", "hrd.fnt");
        CCSize btnSize = btn_start->getContentSize();
        backkey->setPosition(ccp(btnSize.width/2, btnSize.height/2));
        btn_start->addChild(backkey);

        CCMenu* menu = CCMenu::create(btn_start, NULL);
        menu->setPosition(ccp(winSize.width - btnSize.width, winSize.height - btnSize.height/2 - 390));
        m_frame->addChild(menu);

        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        ret = true;
    } while (0);
    return ret;
}

void GameLayer::menuClick(cocos2d::CCObject* object)
{
    CCUserDefault::sharedUserDefault()->setStringForKey("save", m_puzzle->serilize().c_str());
    int steps = m_puzzle->getCurrentStep();
    CCUserDefault::sharedUserDefault()->setStringForKey("save_step", CCString::createWithFormat("%d", steps)->getCString());
    GameScene::instance()->showGameSelectLayer();
}

void GameLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
    //CCLOG("touch begin x=%f, y=%f\n", m_touchPoint.x, m_touchPoint.y);
    if (m_puzzle)
    {
        m_puzzle->touched(m_touchPoint);
    }

    return true;
}

void GameLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint dest = pTouch->getLocation();
    //CCLOG("if %f, %f, sub=%f, sub2=%f\n", fabs(dest.x - m_touchPoint.x),
    //    fabs(dest.y - m_touchPoint.y),
    //    fabs(dest.x - m_touchPoint.x),
    //    fabs(dest.y - m_touchPoint.y));

    if ((fabs(dest.x - m_touchPoint.x) - fabs(dest.y - m_touchPoint.y) > 0)
        && (fabs(dest.x - m_touchPoint.x) > 5))
    { // left or right
        if ((dest.x - m_touchPoint.x) > 0)
        {
            m_puzzle->moveTo(m_touchPoint, MD_RIGHT);
        }
        else
        {
            m_puzzle->moveTo(m_touchPoint, MD_LEFT);
        }
    }
    else if ((fabs(dest.x - m_touchPoint.x) - fabs(dest.y - m_touchPoint.y) < 0)
        && (fabs(dest.y - m_touchPoint.y) > 5))
    { // top or down
        if ((dest.y - m_touchPoint.y) > 0)
        {
            m_puzzle->moveTo(m_touchPoint, MD_TOP);
        }
        else
        {
            m_puzzle->moveTo(m_touchPoint, MD_DOWN);
        }
    }
}

void GameLayer::moveToSuccess(int step)
{
    if (m_currentValue)
    {
        m_currentValue->setCString(CCString::createWithFormat("%d", step)->getCString());
    }
}

void GameLayer::solvedSuccess()
{
    PopView* popView = PopView::create();
    popView->m_currentLevel = m_currentLevel;
    popView->m_parent = this;
    this->addChild(popView, 200);

    CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();

    CCParticleExplosion* m_emitter = CCParticleExplosion::create();
    m_emitter->setPosition(ccp(winSize.width/2, winSize.height/2));

    this->addChild(m_emitter, 300);

    m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("star.png"));
    m_emitter->setAutoRemoveOnFinish(true);

    //ad
    WapsAd::showAd(1);
}

void GameLayer::startGame(int level)
{
    m_currentValue->setCString(CCString::createWithFormat("%d", 0)->getCString());
    m_currentLevel = level;
    CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    CCString* curLevel = CCString::createWithFormat("level_%d", level);
    std::string board = CCUserDefault::sharedUserDefault()->getStringForKey(curLevel->getCString());
    CC_ASSERT(!board.empty());

    if (m_puzzle)
    {
        m_puzzle->detach(this);
        delete m_puzzle;
        m_puzzle = NULL;
    }
    m_puzzle = new KlotskiPuzzle(this);
    m_puzzle->startGame(board.c_str());
    m_puzzle->attach(this);
}

void GameLayer::resumeGame()
{
    if (m_puzzle)
    {
        m_puzzle->detach(this);
        delete m_puzzle;
        m_puzzle = NULL;
    }
    if (m_currentValue)
    {
        m_currentValue->setCString(CCUserDefault::sharedUserDefault()->getStringForKey("save_step", "0").c_str());
    }

    m_puzzle = new KlotskiPuzzle(this);
    const char* step = CCUserDefault::sharedUserDefault()->getStringForKey("save_step", "0").c_str();
    m_puzzle->setCurrentStep(atoi(step));
    m_puzzle->startGame(CCUserDefault::sharedUserDefault()->getStringForKey("save", "").c_str());
    m_puzzle->attach(this);
}

void GameLayer::keyBackClicked(void)
{
    CCLOG("back key click GameLayer");
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

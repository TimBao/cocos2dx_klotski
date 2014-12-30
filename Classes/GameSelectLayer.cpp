#include "GameSelectLayer.h"
#include <math.h>
#include "GameScene.h"
#include "WapsAd.h"

USING_NS_CC;
USING_NS_CC_EXT;

static const int TAG_DISPLAY = 100;

LsTouch::LsTouch()
{
    m_iEventId = 0;
}

LsTouch::~LsTouch()
{
}

bool LsTouch::init()
{
    return true;
}

void LsTouch::setDisplay(CCSprite* dis)
{
    addChild(dis, 0, TAG_DISPLAY);
}

CCSprite* LsTouch::getDisplay()
{
    return dynamic_cast<CCSprite*>(this->getChildByTag(TAG_DISPLAY));
}

void LsTouch::setEventId(int eventId)
{
    m_iEventId = eventId;
}

int LsTouch::getEventId()
{
    return m_iEventId;
}

bool LsTouch::selfCheck(CCTouch* ccTouch, LsTouchEvent* lsTe)
{
    bool bRef = false;
    do {

        CC_BREAK_IF(!containsCCTouchPoint(ccTouch));

        CC_BREAK_IF(!isRunning());

        CC_BREAK_IF(!isVisible());

        bRef = true;
    } while (0);
    return bRef;
}

bool LsTouch::containsCCTouchPoint(CCTouch* ccTouch)
{
    CCNode* dis = getChildByTag(TAG_DISPLAY);
    CCSprite* sprite = dynamic_cast<CCSprite*>(dis);
    CCPoint point = sprite->convertTouchToNodeSpaceAR(ccTouch);
    CCSize s = sprite->getContentSize();//sprite->getTexture()->getContentSize();
    CCRect rect = CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
    return rect.containsPoint(point);
}

bool LsTouch::isParentAllVisible(LsTouchEvent* lsTe)
{
    bool bRef = true;
    CCNode* nLsTe = dynamic_cast<CCNode*>(lsTe);
    CCNode* parent = getParent();
    do
    {
        if (!parent)
        {
            bRef = false;
            break;
        }
        if (nLsTe == parent)
        {
            break;
        }
        if (!parent->isVisible())
        {
            bRef = false;
            break;
        }
        parent = parent->getParent();
    } while (1);

    return bRef;
}

LsTouchEvent::LsTouchEvent()
{
    m_pLsTouches = CCArray::create();
    m_pLsTouches->retain();
}

LsTouchEvent::~LsTouchEvent()
{
    m_pLsTouches->release();
}

void LsTouchEvent::addLsTouch(LsTouch* touch, int eventId)
{
    touch->setEventId(eventId);
    m_pLsTouches->addObject(touch);
}

void LsTouchEvent::removeLsTouch(LsTouch* touch)
{
    m_pLsTouches->removeObject(touch, true);
}

bool LsTouchEvent::sendTouchMessage(CCTouch* ccTouch, int type)
{
    LsTouch* lsTouch = NULL;

    CCObject* pObj = NULL;
    LsTouch* lt = NULL;
    CCARRAY_FOREACH(m_pLsTouches, pObj)
    {
        lt = dynamic_cast<LsTouch*>(pObj);
        if (lt)
        {
            if (lt->selfCheck(ccTouch, this))
            {
                if (lsTouch == NULL)
                {
                    lsTouch = lt;
                }
                else
                {
                    lsTouch = getPriorityTouch(lsTouch, lt);
                }
            }
        }
    }
    touchEventAction(lsTouch, type);
    return true;
}

LsTouch* LsTouchEvent::getPriorityTouch(LsTouch* a, LsTouch* b)
{
    CCNode* nLsTe = dynamic_cast<CCNode*>(this);

    CCNode* allParent = NULL;

    CCNode* nAParent = a;
    CCNode* nBParent = b;
    CCNode* nAChild = NULL;
    CCNode* nBChild = NULL;
    do
    {
        nAChild = nAParent;
        nAParent = nAParent->getParent();
        if (!nAParent) break;

        nBParent = b;
        do
        {
            nBChild = nBParent;
            nBParent = nBParent->getParent();
            if (!nBParent) break;
            if (nAParent == nBParent)
            {
                allParent = nAParent;
                break;
            }
            if (nBParent == nLsTe)
            {
                break;
            }
        } while (1);

        if (allParent)
        {
            break;
        }
        if (nAParent == nLsTe)
        {
            break;
        }
    } while (1);

    if (!nAChild || !nBChild)
    {
        return a;
    }
    if (nAChild->getZOrder() == nBChild->getZOrder())
    {
        return allParent->getChildren()->indexOfObject(nAChild) > allParent->getChildren()->indexOfObject(nBChild)? a: b;
    }
    else
    {
        return nAChild->getZOrder() > nBChild->getZOrder()? a: b;
    }
}

GameSelectLayer::GameSelectLayer()
    : m_curPage(0)
    , m_pageCount(0)
    , m_scrollView(NULL)
{
}

GameSelectLayer::~GameSelectLayer()
{
}

bool GameSelectLayer::init()
{
    bool ret = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());

        CCLayerColor* backLayerColor = CCLayerColor::create(ccc4(25, 25, 25, 125));
        this->addChild(backLayerColor);

        m_scrollView = CCScrollView::create(this->getContentSize());
        m_scrollView->setContainer(getContainLayer());
        m_scrollView->setTouchEnabled(false);
        this->addChild(m_scrollView);

        setCurPageBall();

        WapsAd::showAd(0);

        setKeypadEnabled(true);

        ret = true;
    } while (0);
    return ret;
}

void GameSelectLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void GameSelectLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

void GameSelectLayer::touchEventAction(LsTouch *touch, int type)
{
    const int selectTag = 10001;
    if (type == 0 && touch)
    {
        m_scrollView->getContainer()->removeChildByTag(selectTag);

        CCSprite* sprite = CCSprite::createWithSpriteFrameName("sel.png");
        sprite->setScaleX(1.5f);
        sprite->setScaleY(2.5f);
        sprite->setPosition(touch->getPosition());
        m_scrollView->getContainer()->addChild(sprite, 1, selectTag);
    }
    else
    {
        m_scrollView->getContainer()->removeChildByTag(selectTag);
    }
    if (type == 1 && touch)
    {
        GameScene::instance()->startGame(touch->getEventId() + 1);
    }
}

bool GameSelectLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());

    m_touchPoint = pTouch->getLocation();
    m_touchOffset = m_scrollView->getContentOffset();

    sendTouchMessage(pTouch, 0);
    return true;
}

void GameSelectLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint movePoint = pTouch->getLocation();
    float distance = movePoint.x - m_touchPoint.x;

    CCPoint adjustPoint = ccp(m_touchOffset.x + distance, 0);
    m_scrollView->setContentOffset(adjustPoint, false);
}

void GameSelectLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint endPoint = pTouch->getLocation();
    float distance = endPoint.x - m_touchPoint.x;
    float distanceY = endPoint.y - m_touchPoint.y;
    if (fabs(distance) < 3 && fabs(distanceY) < 3)
    {
        sendTouchMessage(pTouch, 1);

    }
    else if (fabs(distance) > 140)
    {
        adjustScrollView(distance);
        setCurPageBall();
    }
    else
    {
        adjustScrollView(0);
    }

    sendTouchMessage(pTouch, 2);
}

void GameSelectLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    adjustScrollView(0);
}

void GameSelectLayer::adjustScrollView(float offset)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (offset < 0)
    {
        m_curPage ++;
    }
    else if (offset > 0)
    {
        m_curPage --;
    }
    if (m_curPage < 0)
    {
        m_curPage = 0;
    }
    else if (m_curPage > m_pageCount - 1)
    {
        m_curPage = m_pageCount - 1;
    }

    CCPoint adjustPoint = ccp(-winSize.width * m_curPage , 0);
    m_scrollView->setContentOffsetInDuration(adjustPoint, 0.2f);
}

CCLayer* GameSelectLayer::getContainLayer()
{
    int levelCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("total");
    int widthCount = 4;
    int heightCount = 5;
    m_pageCount = ceil(levelCount / (widthCount * heightCount) + 0.5f);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint pointCenter = ccp(winSize.width / 2, winSize.height / 2);

    CCLayer* layer = CCLayer::create();
    layer->setPosition(CCPointZero);

    for (int i = 0; i < m_pageCount; i++)
    {
        CCSprite* frame = CCSprite::create("frame.png");
        frame->setPosition(ccpAdd(pointCenter, ccp(winSize.width * i, 0)));
        layer->addChild(frame, 0, 0);
    }

    for (int i = 0; i < levelCount; i++)
    {
        CCNode* level = getSpriteByLevel(i);

        int curPageIndex = i / (widthCount * heightCount);
        float xOffset = i % 4 * 160 + 120;
        float yOffset = winSize.height - ((i % (widthCount * heightCount)) / widthCount * 90 + 100) - 280;
        //float yOffset = winSize.height - ((i % (widthCount * heightCount)) / widthCount * 90 + 100) - 180;

        level->setPosition(ccp(xOffset + winSize.width * curPageIndex, yOffset));
        layer->addChild(level ,2, i);
    }

    layer->setContentSize(CCSizeMake(winSize.width * m_pageCount, winSize.height));

    return layer;
}

CCNode* GameSelectLayer::getSpriteByLevel(int level){

    CCSprite* sprite = CCSprite::createWithSpriteFrameName("level.png");
    CCString* str = CCString::createWithFormat("%d", level + 1);
    CCLabelBMFont* label = CCLabelBMFont::create(str->getCString(),"hrd.fnt");
    label->setPosition(ccp(40, 45));
    sprite->addChild(label);

    LsTouch* touch = LsTouch::create();
    touch->setDisplay(sprite);
    this->addLsTouch(touch, level);

    return touch;
}

void GameSelectLayer::setCurPageBall()
{
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    const int tagNodeBall = 1002;
    float fs = 0.2f;
    float scale = 1.0f;

    CCNode* layer = this->getChildByTag(tagNodeBall);
    if (layer)
    {
        CCLayer* lyBall = dynamic_cast<CCLayer*>(layer);
        for (int i = 0; i < m_pageCount; i++)
        {
            CCNode* n = lyBall->getChildByTag(i);
            if (n)
            {
                CCSprite* sprite = dynamic_cast<CCSprite*>(n);
                if (m_curPage == i)
                {
                    sprite->runAction(
                        CCScaleTo::create(fs,
                        1.0f));
                }
                else
                {
                    sprite->runAction(
                        CCScaleTo::create(0.5,
                        scale));
                }
            }
        }
    }
    else
    {
        CCLayer* lyBall = CCLayer::create();
        this->addChild(lyBall, 0, tagNodeBall);

        int ball_width = 30;
        for (int i = 0; i < m_pageCount; i++)
        {
            CCSprite* sprite = CCSprite::createWithSpriteFrameName("ball.png");
            sprite->setPosition(ccp(ball_width*i, ball_width / 2));
            sprite->setScale(scale);
            lyBall->addChild(sprite, 0, i);
            if (i == m_curPage)
            {
                sprite->runAction(CCScaleTo::create(fs, 1.0f));
            }
        }
        lyBall->setPosition(
            size.width / 2 - m_pageCount/2*ball_width,
            30);
    }
}

void GameSelectLayer::keyBackClicked(void)
{
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

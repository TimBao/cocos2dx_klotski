#include "Block.h"
#include <string>
#include "KlotskiSolver.h"

USING_NS_CC;

static const float BLOCK_WIDTH = 160.0;

Block::Block(BlockType type, int x, int y)
    : m_type(type)
    , m_x(x)
    , m_y(y)
    , m_sprite(NULL)
{
    const char* spriteName = "";
    const char* selectedName = "";
    if (m_type == BT_Single)
    {
        spriteName = "x.png";
        selectedName = "x_s.png";
    }
    else if (m_type == BT_Horizon)
    {
        static int j = 1;
        if (j > 4)
        {
            j = 1;
        }
        spriteName = CCString::createWithFormat("h%d.png", j)->getCString();
        selectedName = CCString::createWithFormat("h%d_s.png", j)->getCString();
        j++;
    }
    else if (m_type == BT_Square)
    {
        spriteName = "baby.png";
        selectedName = "baby_s.png";
    }
    else if (m_type == BT_Vertical)
    {
        static int i = 1;
        if (i > 4)
        {
            i = 1;
        }
        spriteName = CCString::createWithFormat("v%d.png", i)->getCString();
        selectedName = CCString::createWithFormat("v%d_s.png", i)->getCString();
        ++i;
    }

    CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = ccp(winSize.width/2 - 320, winSize.height/2 + 310);

    m_sprite = CCSprite::createWithSpriteFrameName(spriteName);
    CC_ASSERT(m_sprite != NULL);
    m_sprite->retain();
    m_sprite->setAnchorPoint(ccp(0, 0));
    m_sprite->setPosition(ccp(BLOCK_WIDTH*m_y + origin.x, origin.y - BLOCK_WIDTH*m_x - m_sprite->getContentSize().height));

    m_normalFrame = m_sprite->displayFrame();
    m_normalFrame->retain();
    CCSpriteFrameCache* cacher = CCSpriteFrameCache::sharedSpriteFrameCache();
    m_selectedFrame = cacher->spriteFrameByName(selectedName);
    m_selectedFrame->retain();
}

Block::~Block()
{
    m_sprite->release();
    m_normalFrame->release();
    m_selectedFrame->release();
}

bool Block::attach(cocos2d::CCNode* parent)
{
    if (!parent)
    {
        return false;
    }

    parent->addChild(m_sprite);
    return true;
}

void Block::detach(cocos2d::CCNode* parent)
{
    if (!parent)
    {
        return;
    }

    parent->removeChild(m_sprite);
}

void Block::select()
{
    m_sprite->setDisplayFrame(m_selectedFrame);
}

void Block::unselect()
{
    m_sprite->setDisplayFrame(m_normalFrame);
}

void Block::moveTo(int x, int y)
{
    CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = ccp(winSize.width/2 - 320, winSize.height/2 + 310);

    CCPoint destination = ccp(BLOCK_WIDTH*y + origin.x, origin.y - BLOCK_WIDTH*x - m_sprite->getContentSize().height);
    //CCMoveTo* move = CCMoveTo::create(0.5f, destination);
    //m_sprite->runAction(move);
    m_sprite->setPosition(destination);
    m_x = x;
    m_y = y;
}

void Block::mask(KlotskiSolver* mask)
{
    mask->setMask(static_cast<int>(m_type), m_x, m_y);
    switch (m_type)
    {
    case BT_Horizon:
        mask->setMask(static_cast<int>(m_type), m_x, m_y+1);
        break;
    case BT_Vertical:
        mask->setMask(static_cast<int>(m_type), m_x+1, m_y);
        break;
    case BT_Square:
        mask->setMask(static_cast<int>(m_type), m_x, m_y+1);
        mask->setMask(static_cast<int>(m_type), m_x+1, m_y);
        mask->setMask(static_cast<int>(m_type), m_x+1, m_y+1);
        break;
    default:
        CC_ASSERT(m_type == BT_Single);
        break;
    }
}

int Block::getPositionBottom()
{
    int bottom = m_x;
    switch (m_type)
    {
    case BT_Vertical:
    case BT_Square:
        bottom++;
        break;
    default:
        break;
    }
    return bottom;
}

int Block::getPositionRight()
{
    const static int delta[] = { 0, 0, 0, 1, 1, };
    return m_y + delta[static_cast<int>(m_type)];
}

#include "KlotskiPuzzle.h"
#include "Block.h"
#include "KlotskiSolver.h"

USING_NS_CC;

KlotskiPuzzle::KlotskiPuzzle(KlotskiPuzzleListener* listener)
    : m_step(0)
    , m_listener(listener)
{
    for (int i = 0; i < BLOCK_NUM; ++i)
    {
        memset(m_blocks, 0, sizeof(Block));
    }
}

KlotskiPuzzle::~KlotskiPuzzle()
{
    for (int i = 0; i < BLOCK_NUM; ++i)
    {
        CC_SAFE_DELETE(m_blocks[i]);
    }
}

void KlotskiPuzzle::init()
{
}

void KlotskiPuzzle::startGame(const char* board)
{
    int len = strlen(board);
    CC_ASSERT(len == 30);
    char currentboard[BLOCK_NUM][3] = {'0'};
    for (int i = 0; i < BLOCK_NUM; ++i)
    {
        currentboard[i][0] = board[i*3];
        currentboard[i][1] = board[i*3+1];
        currentboard[i][2] = board[i*3+2];
    }
    BlockType type = BT_Invalid;
    for (int i = 0; i < BLOCK_NUM; ++i)
    {
        switch (currentboard[i][0])
        {
        case 'V':
            type = BT_Vertical;
            break;
        case 'Q':
            type = BT_Square;
            break;
        case 'H':
            type = BT_Horizon;
            break;
        case 'S':
            type = BT_Single;
            break;
        default:
            break;
        }
        CC_ASSERT(type != BT_Invalid);
        m_blocks[i] = new Block(type, (int)currentboard[i][1] - 48, (int)currentboard[i][2] - 48);
    }
}

bool KlotskiPuzzle::attach(cocos2d::CCNode* parent)
{
    for (int i = 0; i < BLOCK_NUM; ++i)
    {
        CC_ASSERT(m_blocks[i] != NULL);
        if (!m_blocks[i]->attach(parent))
        {
            return false;
        }
    }
    return true;
}

void KlotskiPuzzle::detach(cocos2d::CCNode* parent)
{
    for (int i = 0; i < BLOCK_NUM; ++i)
    {
        CC_ASSERT(m_blocks[i] != NULL);
        m_blocks[i]->detach(parent);
    }
}

void KlotskiPuzzle::touched(cocos2d::CCPoint point)
{
    for (int i = 0; i < BLOCK_NUM; ++i)
    {
        if (m_blocks[i]->getBoundingBox().containsPoint(point))
        {
            m_blocks[i]->select();
        }
        else
        {
            m_blocks[i]->unselect();
        }
    }
}

void KlotskiPuzzle::moveTo(cocos2d::CCPoint point, MoveDirection dir)
{
    for (int i = 0; i < BLOCK_NUM; ++i)
    {
        if (m_blocks[i]->getBoundingBox().containsPoint(point))
        {
            KlotskiSolver mask = toMask();
            switch (dir)
            {
            case MD_LEFT:
                {
                    if (m_blocks[i]->getPositionY() > 0
                        && mask.isEmpty(m_blocks[i]->getPositionX(), m_blocks[i]->getPositionY() - 1)
                        && mask.isEmpty(m_blocks[i]->getPositionBottom(), m_blocks[i]->getPositionY() - 1))
                    {
                        m_step++;
                        m_blocks[i]->moveTo(m_blocks[i]->getPositionX(), m_blocks[i]->getPositionY() - 1);
                        if (m_listener)
                        {
                            m_listener->moveToSuccess(m_step);
                        }
                    }
                }
                break;
            case MD_RIGHT:
                {
                    if (m_blocks[i]->getPositionY() < 3
                        && mask.isEmpty(m_blocks[i]->getPositionX(), m_blocks[i]->getPositionRight() + 1)
                        && mask.isEmpty(m_blocks[i]->getPositionBottom(), m_blocks[i]->getPositionRight() + 1))
                    {
                        m_step++;
                        m_blocks[i]->moveTo(m_blocks[i]->getPositionX(), m_blocks[i]->getPositionY() + 1);
                        if (m_listener)
                        {
                            m_listener->moveToSuccess(m_step);
                        }
                    }
                }
                break;
            case MD_TOP:
                {
                    if (m_blocks[i]->getPositionX() > 0
                         && mask.isEmpty(m_blocks[i]->getPositionX() - 1, m_blocks[i]->getPositionY())
                         && mask.isEmpty(m_blocks[i]->getPositionX() - 1, m_blocks[i]->getPositionRight()))
                    {
                        m_step++;
                        m_blocks[i]->moveTo(m_blocks[i]->getPositionX() - 1, m_blocks[i]->getPositionY());
                        if (m_listener)
                        {
                            m_listener->moveToSuccess(m_step);
                        }
                    }
                }
                break;
            case MD_DOWN:
                {
                    if (m_blocks[i]->getPositionX() < 4
                        && mask.isEmpty(m_blocks[i]->getPositionBottom() + 1, m_blocks[i]->getPositionY())
                        && mask.isEmpty(m_blocks[i]->getPositionBottom() + 1, m_blocks[i]->getPositionRight()))
                    {
                        m_step++;
                        m_blocks[i]->moveTo(m_blocks[i]->getPositionX() + 1, m_blocks[i]->getPositionY());
                        if (m_listener)
                        {
                            m_listener->moveToSuccess(m_step);
                        }
                    }
                }
                break;
            default:
                break;
            }
            break;
        }
    }

    if (isSolved())
    {
        if (m_listener)
        {
            m_listener->solvedSuccess();
        }
    }
}

bool KlotskiPuzzle::isSolved()
{
    bool bRet = false;
    for (int i = 0; i < BLOCK_NUM; ++i)
    {
        if (m_blocks[i]->getType() == BT_Square)
        {
            bRet = m_blocks[i]->getPositionX() == 3 && m_blocks[i]->getPositionY() == 1;
            break;
        }
        
    }
    return bRet;
}

KlotskiSolver KlotskiPuzzle::toMask()
{
    KlotskiSolver mask;
    for (int i = 0; i < BLOCK_NUM; ++i)
    {
        m_blocks[i]->mask(&mask);
    }
    return mask;
}

std::string KlotskiPuzzle::serilize()
{
    char currentboard[BLOCK_NUM*3 + 1] = {'0'};
    for (int i = 0; i < BLOCK_NUM; ++i)
    {
        switch (m_blocks[i]->getType())
        {
        case BT_Vertical:
            currentboard[i*3] = 'V';
            break;
        case BT_Square:
            currentboard[i*3] = 'Q';
            break;
        case BT_Horizon:
            currentboard[i*3] = 'H';
            break;
        case BT_Single:
            currentboard[i*3] = 'S';
            break;
        default:
            break;
        }
        currentboard[i*3 + 1] = m_blocks[i]->getPositionX()+48;
        currentboard[i*3 + 2] = m_blocks[i]->getPositionY()+48;
    }
    return std::string(currentboard);
}
#include "KlotskiSolver.h"
#include <string.h>

#include "cocos2d.h"

KlotskiSolver::KlotskiSolver()
{
    memset(m_mask, 0, sizeof(m_mask));
}

KlotskiSolver::~KlotskiSolver()
{
}

void KlotskiSolver::setMask(int value, int x, int y)
{
    CC_ASSERT(x >= 0 && x < 5);
    CC_ASSERT(y >= 0 && y < 4);
    m_mask[x][y] = value;
}

bool KlotskiSolver::isEmpty(int x, int y)
{
    return m_mask[x][y] == 0;
}

void KlotskiSolver::print()
{
    char temp[5]={0};
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 4; j++)
        {
            sprintf(&temp[j], "%c", m_mask[i][j]+'0');
        }
        CCLOG(temp);

    }
    CCLOG("\n");
}
#include "stdafx.h"
#include "BattleArea.h"

bool BattleArea::BeginBattle()
{
    //戦闘中やクリア済みの区画では、敵の出現を繰り返さない。
    if (m_isBattleActive || m_isCleared)
    {
        return false;
    }

    m_isBattleActive = true;
    return true;
}

void BattleArea::CompleteBattle()
{
    if (m_isBattleActive)
    {
        m_isBattleActive = false;
        m_isCleared = true;
    }
}

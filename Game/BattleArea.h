#pragma once

class BattleArea
{
public:
    bool BeginBattle();
    void CompleteBattle();

    bool IsBattleActive() const 
    {
        return m_isBattleActive;
    }
    bool IsCleared() const 
    {
        return m_isCleared; 
    }

private:
    bool m_isBattleActive = false;
    bool m_isCleared = false;
};

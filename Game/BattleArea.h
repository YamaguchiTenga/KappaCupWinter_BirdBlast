#pragma once
#include <memory>
#include "EnemySpawner.h"

class BattleArea
{
public:
    void Update(const Vector3& playerPosition, EnemyManager& enemyManager, float deltaTime);

    void InitializeGate();
    void Render(RenderContext& rc);

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
    EnemySpawner m_enemySpawner;
    ModelRender m_gateRender;
    std::unique_ptr<PhysicsStaticObject> m_gateCollision;
    bool m_isBattleActive = false;
    bool m_isCleared = false;
};

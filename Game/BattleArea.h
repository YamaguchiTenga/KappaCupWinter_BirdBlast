#pragma once
#include <memory>
#include "EnemySpawner.h"

class BattleArea
{
public:
    void Update(const Vector3& playerPosition, EnemyManager& enemyManager, float deltaTime);

    void Initialize(float entryStartZ, float entryEndZ, float gateZ,
        const std::vector<EnemySpawn>& spawns, const BattleArea* prerequisite = nullptr);
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
    float m_entryStartZ = 0.0f;
    float m_entryEndZ = 0.0f;
    std::vector<EnemySpawn> m_spawns;
    const BattleArea* m_prerequisite = nullptr;
    EnemySpawner m_enemySpawner;
    ModelRender m_gateRender;
    ModelRender m_barrierRender;
    std::unique_ptr<PhysicsStaticObject> m_gateCollision;
    bool m_isBattleActive = false;
    bool m_isCleared = false;
};

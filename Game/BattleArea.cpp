#include "stdafx.h"
#include "BattleArea.h"

void BattleArea::Initialize(float entryStartZ, float entryEndZ, float gateZ,
    const std::vector<EnemySpawn>& spawns, const BattleArea* prerequisite)
{
    m_entryStartZ = entryStartZ;
    m_entryEndZ = entryEndZ;
    m_spawns = spawns;
    m_prerequisite = prerequisite;

    m_gateRender.Init("Assets/modelData/Floor/Forest/ForestGate.tkm", nullptr, 0, enModelUpAxisY);
    m_gateRender.SetPosition(0.0f, 0.0f, gateZ);
    m_gateRender.Update();
}

void BattleArea::Render(RenderContext& rc)
{
    if (IsBattleActive())
    {
        m_gateRender.Draw(rc);
    }
}

void BattleArea::Update(const Vector3& playerPosition, EnemyManager& enemyManager, float deltaTime)
{
    m_enemySpawner.Update(enemyManager, deltaTime);

    if (m_isBattleActive && m_enemySpawner.AreAllSpawnedEnemiesDefeated())
    {
        CompleteBattle();
        OutputDebugStringW(L"\u6226\u95D8\u30AF\u30EA\u30A2\n");
    }

    // Entering the clearing from either side of the trail starts the battle.
    if (playerPosition.z >= m_entryStartZ && playerPosition.z <= m_entryEndZ && BeginBattle())
    {
        m_enemySpawner.BeginSpawn(m_spawns);
    }
}

bool BattleArea::BeginBattle()
{
    //戦闘中やクリア済みの区画では、敵の出現を繰り返さない。
    if (m_isBattleActive || m_isCleared ||
        (m_prerequisite != nullptr && !m_prerequisite->IsCleared()))
    {
        return false;
    }

    m_gateCollision = std::make_unique<PhysicsStaticObject>();
    m_gateCollision->CreateFromModel(m_gateRender.GetModel(), m_gateRender.GetModel().GetWorldMatrix());
    m_isBattleActive = true;
    return true;
}

void BattleArea::CompleteBattle()
{
    if (m_isBattleActive)
    {
        m_gateCollision.reset();
        m_isBattleActive = false;
        m_isCleared = true;
    }
}

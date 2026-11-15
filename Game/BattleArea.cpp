#include "stdafx.h"
#include "BattleArea.h"

void BattleArea::InitializeGate()
{
    m_gateRender.Init("Assets/modelData/Floor/Forest/ForestGate.tkm", nullptr, 0, enModelUpAxisY);
    m_gateRender.SetPosition(0.0f, 0.0f, 700.0f);
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

    const Vector3 areaCenter{ 0.0f, 0.0f, 450.0f };
    // Entering the clearing from either side of the trail starts the battle.
    if (playerPosition.z >= 300.0f && playerPosition.z <= 600.0f && BeginBattle())
    {
        m_enemySpawner.BeginSpawn(areaCenter);
    }
}

bool BattleArea::BeginBattle()
{
    //戦闘中やクリア済みの区画では、敵の出現を繰り返さない。
    if (m_isBattleActive || m_isCleared)
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

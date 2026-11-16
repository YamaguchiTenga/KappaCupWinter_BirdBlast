#include "stdafx.h"
#include "EnemySpawner.h"
#include "EnemyManager.h"
#include "EffectManager.h"

void EnemySpawner::BeginSpawn(const std::vector<EnemySpawn>& spawns)
{
    m_spawns = spawns;
    m_spawnTimeRemaining = 0.4f;

    const Vector3 smokeScale{ 20.0f, 20.0f, 20.0f };
    for (const EnemySpawn& spawn : m_spawns)
    {
        EffectManager::PlayEffect(EffectManager::Effect::EnemyDeathSmoke, spawn.position, smokeScale);
    }
}

void EnemySpawner::Update(EnemyManager& enemyManager, float deltaTime)
{
    if (m_spawnTimeRemaining <= 0.0f)
    {
        return;
    }

    m_spawnTimeRemaining -= deltaTime;
    if (m_spawnTimeRemaining <= 0.0f)
    {
        for (const EnemySpawn& spawn : m_spawns)
        {
            m_spawnedEnemies.push_back(enemyManager.SpawnEnemy(spawn.position, spawn.type));
        }
    }
}

bool EnemySpawner::AreAllSpawnedEnemiesDefeated() const
{
    if (m_spawnedEnemies.empty())
    {
        return false;
    }

    for (const Enemy* enemy : m_spawnedEnemies)
    {
        if (enemy->IsActive())
        {
            return false;
        }
    }

    return true;
}

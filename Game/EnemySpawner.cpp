#include "stdafx.h"
#include "EnemySpawner.h"
#include "EnemyManager.h"
#include "EffectManager.h"

void EnemySpawner::BeginSpawn(const Vector3& areaCenter)
{
    m_leftSpawnPosition = areaCenter + Vector3(-100.0f, 0.0f, 100.0f);
    m_rightSpawnPosition = areaCenter + Vector3(100.0f, 0.0f, 100.0f);
    m_spawnTimeRemaining = 0.4f;

    const Vector3 smokeScale{ 20.0f, 20.0f, 20.0f };
    EffectManager::PlayEffect(EffectManager::Effect::EnemyDeathSmoke, m_leftSpawnPosition, smokeScale);
    EffectManager::PlayEffect(EffectManager::Effect::EnemyDeathSmoke, m_rightSpawnPosition, smokeScale);
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
        m_spawnedEnemies.push_back(enemyManager.SpawnEnemy(m_leftSpawnPosition));
        m_spawnedEnemies.push_back(enemyManager.SpawnEnemy(m_rightSpawnPosition));
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

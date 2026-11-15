#pragma once
#include <vector>

class Enemy;
class EnemyManager;

class EnemySpawner
{
public:
    void BeginSpawn(const Vector3& areaCenter);
    void Update(EnemyManager& enemyManager, float deltaTime);
    bool AreAllSpawnedEnemiesDefeated() const;

private:
    Vector3 m_leftSpawnPosition = Vector3::Zero;
    Vector3 m_rightSpawnPosition = Vector3::Zero;
    float m_spawnTimeRemaining = 0.0f;

    // このスポナーが出した敵を、後で全滅判定に使うために記録する。
    std::vector<Enemy*> m_spawnedEnemies;
};

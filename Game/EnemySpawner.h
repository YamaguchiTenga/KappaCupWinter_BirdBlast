#pragma once
#include <vector>

class Enemy;
class EnemyManager;
enum class EnemyType;

struct EnemySpawn
{
    Vector3 position;
    EnemyType type;
};

class EnemySpawner
{
public:
    void BeginSpawn(const std::vector<EnemySpawn>& spawns);
    void Update(EnemyManager& enemyManager, float deltaTime);
    bool AreAllSpawnedEnemiesDefeated() const;

private:
    std::vector<EnemySpawn> m_spawns;
    float m_spawnTimeRemaining = 0.0f;

    // このスポナーが出した敵を、後で全滅判定に使うために記録する。
    std::vector<Enemy*> m_spawnedEnemies;
};

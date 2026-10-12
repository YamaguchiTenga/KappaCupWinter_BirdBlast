#pragma once
#include "stdafx.h"
#include <vector>
#include"Enemy.h"

class Enemy;

class EnemyManager : public IGameObject
{
public:
	EnemyManager();
	~EnemyManager();

	bool Start();

	void Update();

	void SpawnEnemy(const Vector3& position);

	const std::vector<Enemy*>& GetEnemies()const
	{
		return m_enemies;
	}
private:
	std::vector<Enemy*> m_enemies;
};


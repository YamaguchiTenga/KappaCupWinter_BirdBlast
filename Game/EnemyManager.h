#pragma once
#include "stdafx.h"
#include <vector>
#include"Enemy.h"

class Enemy;

enum class EnemyType
{
	Mushroom,
	GoblinArcher
};

class EnemyManager : public IGameObject
{
public:
	EnemyManager();
	~EnemyManager();

	bool Start();

	void Update();

	Enemy* SpawnEnemy(const Vector3& position, EnemyType type = EnemyType::Mushroom);

public:
	const std::vector<Enemy*>& GetEnemies()const
	{
		return m_enemies;
	}
private:
	std::vector<Enemy*> m_enemies;
};


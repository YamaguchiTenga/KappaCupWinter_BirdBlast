#include "stdafx.h"
#include "EnemyManager.h"
#include "MushroomEnemy.h"
#include "GoblinArcherEnemy.h"

bool EnemyManager::Start()
{
	SpawnEnemy(Vector3(300.0f, 0.0f, 300.0f));
	SpawnEnemy(Vector3(100.0f, 0.0f, 300.0f));
	SpawnEnemy(Vector3(-300.0f, 0.0f, 300.0f));


	return true;
}

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
	for (Enemy* enemy : m_enemies)
	{
		DeleteGO(enemy);
	}
}

void EnemyManager::Update()
{

}

//敵を出現させるのに必要な情報を入れ,出現させる
Enemy* EnemyManager::SpawnEnemy(const Vector3& position, EnemyType type)
{
	Enemy* enemy = nullptr;
	if (type == EnemyType::GoblinArcher)
	{
		enemy = NewGO<GoblinArcherEnemy>(0, "enemy");
	}
	else
	{
		enemy = NewGO<MushroomEnemy>(0, "enemy");
	}

	enemy->SetPosition(position);

	m_enemies.push_back(enemy);//m_enemiesの一覧の末尾に敵のポインタを追加する
	return enemy;
}
#include "stdafx.h"
#include "EnemyManager.h"
#include"Enemy.h"

bool EnemyManager::Start()
{
	SpawnEnemy(Vector3(0.0f, 0.0f, 100.0f));
	SpawnEnemy(Vector3(100.0f, 0.0f, 0.0f));
	SpawnEnemy(Vector3(100.0f, 0.0f, 100.0f));
	return true;
}

EnemyManager::EnemyManager()
{

}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Update()
{

}

//敵を出現させるのに必要な情報を入れ,出現させる
void EnemyManager::SpawnEnemy(const Vector3& position)
{
	Enemy* enemy = NewGO<Enemy>(0, "enemy");

	enemy->SetPosition(position);

	m_enemies.push_back(enemy);//m_enemiesの一覧の末尾に敵のポインタを追加する
}
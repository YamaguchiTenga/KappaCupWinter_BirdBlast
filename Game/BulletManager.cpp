#include "stdafx.h"
#include "BulletManager.h"
#include "Bullet.h"

bool BulletManager::Start()
{
	for (int i = 0;i < BulletCount;i++)
	{
		m_bullets[i] = NewGO<Bullet>(0);
	}
	return true;
}

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
	for (int i = 0;i < BulletCount;i++)
	{
		DeleteGO(m_bullets[i]);
	}
}

void BulletManager::RequestFire(const Vector3& position, const Vector3& moveSpeed, BulletTeam team)
{
	for (int i = 0; i < BulletCount; i++)//弾の数だけ調べる
	{
		if (!m_bullets[i]->isActive())//弾がactiveじゃなければ
		{
			m_bullets[i]->Fire(position, moveSpeed,team);//BulletクラスのFireを呼び出す。引数はこの関数とほぼ同じ形にしているので、そのまま渡して大丈夫
			return;//一発しか撃たないようにリターンを入れる
		}
	}
}

void BulletManager::Update()
{

}
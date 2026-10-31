#include "stdafx.h"
#include "BulletManager.h"
#include "Bullet.h"
#include "LaserBullet.h"
#include "PowerLaserBullet.h"
#include"BombBullet.h"

bool BulletManager::Start()
{
	for (int i = 0;i < BulletCount;i++)
	{
		m_enemyBullets[i] = NewGO<Bullet>(0);
	}

	for (int i = 0;i < BulletCount;i++)
	{
		m_laserBullets[i] = NewGO<LaserBullet>(0);
	}

	m_powerLaserBullet = NewGO<PowerLaserBullet>(0);

	for (int i = 0;i < BulletCount;i++)
	{
		m_bombBullets[i] = NewGO<BombBullet>(0);
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
		DeleteGO(m_enemyBullets[i]);
	}

	for (int i = 0;i < BulletCount;i++)
	{
		DeleteGO(m_laserBullets[i]);
	}

	DeleteGO(m_powerLaserBullet);

	for (int i = 0;i < BulletCount;i++)
	{
		DeleteGO(m_bombBullets[i]);
	}
}

bool BulletManager::RequestFire(const Vector3& position, const Vector3& moveSpeed, BulletTeam team, BulletType type)
{
	if (type == BulletType::EnemyShot)
	{
		//基本弾。敵の通常の攻撃とする
		for (int i = 0; i < BulletCount; i++)//弾の数だけ調べる
		{
			if (!m_enemyBullets[i]->isActive())//弾がactiveじゃなければ
			{
				m_enemyBullets[i]->Fire(position, moveSpeed, team);//BulletクラスのFireを呼び出す。引数はこの関数とほぼ同じ形にしているので、そのまま渡して大丈夫
				return true;//一発しか撃たないようにリターンを入れる
			}
		}
	}
	//ここからそれぞれの武器の処理
	else if (type == BulletType::Laser)
	{
		for (int i = 0; i < BulletCount; i++)
		{
			if (!m_laserBullets[i]->isActive())//継承してるからisActiveもFireも使える
			{
				m_laserBullets[i]->Fire(position, moveSpeed, team);
				return true;
			}
		}
	}

	else if (type == BulletType::Bomb)
	{
		for (int i = 0; i < BulletCount; i++)
		{
			if (!m_bombBullets[i]->isActive())//継承してるからisActiveもFireも使える
			{
				m_bombBullets[i]->Fire(position, moveSpeed, team);
				return true;
			}
		}
	}
	return false;//全部調べても空きがなかった。発射できなかった
}

bool BulletManager::RequestPowerLaser(const Vector3& position, const Vector3& direction)
{
	if (!m_powerLaserBullet->IsActive())
	{
		m_powerLaserBullet->Fire(position, direction);
		return true;
	}

	return false;
}

bool BulletManager::IsPowerLaserFiring() const
{
	return m_powerLaserBullet->IsFiring();
}

void BulletManager::Update()
{

}
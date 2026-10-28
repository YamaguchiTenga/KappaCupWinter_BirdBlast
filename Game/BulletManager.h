#pragma once
#include "Bullet.h"

class Bullet;

class BulletManager : public IGameObject
{
public:
	BulletManager();
	~BulletManager();

	bool Start();

	void Update();

	//発射ができたかどうかの判断のためbool型にする
	bool RequestFire(const Vector3& position, const Vector3& moveSpeed, BulletTeam team, BulletType type);
private:
	static const int BulletCount = 10;//配列のサイズに使える定数にしている

	Bullet* m_laserBullets[BulletCount] = {};
	Bullet* m_enemyBullets[BulletCount] = {};
};


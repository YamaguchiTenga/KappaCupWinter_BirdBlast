#pragma once

//弾を生成・保持し、再利用するためのプールを管理する。
//発射依頼を受け、指定された種類の待機中の弾を発射する。

#include "Bullet.h"

class Bullet;
class PowerLaserBullet;
class BombBullet;

class BulletManager : public IGameObject
{
public:
	BulletManager();
	~BulletManager();

	bool Start();

	void Update();

	//発射ができたかどうかの判断のためbool型にする
	bool RequestFire(const Vector3& position, const Vector3& moveSpeed, BulletTeam team, BulletType type);

	//パワー状態のレーザー
	bool RequestPowerLaser(const Vector3& position, const Vector3& direction);
	bool IsPowerLaserFiring() const;
private:
	static const int BulletCount = 10;//配列のサイズに使える定数にしている

	Bullet* m_laserBullets[BulletCount] = {};
	Bullet* m_enemyBullets[BulletCount] = {};
	BombBullet* m_bombBullets[BulletCount] = {};
	PowerLaserBullet* m_powerLaserBullet = nullptr;
};


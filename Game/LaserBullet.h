#pragma once
#include"Bullet.h"
class LaserBullet : public Bullet
{
protected:
	void Configure() override;
	void OnHitEnemy(Enemy* enemy) override;
};


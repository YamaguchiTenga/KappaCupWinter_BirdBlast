#pragma once
#include"Bullet.h"
class LaserBullet : public Bullet
{
protected:
	void Configure() override;
	void Move(float deltaTime) override;
	void OnHitEnemy(Enemy* enemy) override;
};


#pragma once
#include"Bullet.h"
class BombBullet : public Bullet
{
protected:
	void Configure() override;

	void Move(float deltaTIme) override;

private:
	float m_gravity = 1000.0f;
};


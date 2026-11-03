#pragma once
#include"Bullet.h"

class BombBullet : public Bullet
{
public:
	~BombBullet();
	bool Start() override;
protected:
	void Configure() override;

	void Move(float deltaTIme) override;

	void OnHitEnemy(Enemy* enemy)override;

	void Explosion();
private:
	float m_explosionRadius = 100.0f;
	int m_damage = 1;
private:
	CollisionObject* m_explosionCollision = nullptr;
};


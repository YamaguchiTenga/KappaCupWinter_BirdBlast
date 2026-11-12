#pragma once
#include"Bullet.h"

class BombBullet : public Bullet
{
public:
	~BombBullet();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

public:
	void SetPower(bool isPower)
	{
		m_isPower = isPower;
	}

public:
	static const Vector3 CollisionSize;
protected:
	void Configure() override;

	void Move(float deltaTIme) override;

	void OnHitEnemy(Enemy* enemy)override;
	void OnLifetimeExpired() override;

	void Explosion();
private:
	float m_explosionRadius = 100.0f;
	float m_powerExplosionRadius = 300.0f;
	int m_damage = 1;

	bool m_isPower = false;//発射時にPOWERボムだったか

private:
	CollisionObject* m_explosionCollision = nullptr;
	ModelRender m_powerModelRender;
};


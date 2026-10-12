#pragma once
#include "Actor.h"

class EnemyManager;

class Bullet : public Actor
{
public:
	Bullet();
	~Bullet();

	bool Start();

	void Update();

	void Render(RenderContext& rc);

	void Fire(const Vector3& position, const Vector3& moveSpeed);

	void Deactivate();//停止させるという意味

	void CheckHit();

	bool isActive()const
	{
		return m_isActive;
	}

private:
	CollisionObject* m_collisionObject = nullptr;
	Vector3 m_moveSpeed = Vector3::Zero;
	Vector3 m_collisionSize = Vector3::Zero;
	bool m_isActive = false;
	float m_activeTimer = 0.0f;
	float m_limitTime = 0.0f;

	EnemyManager* m_enemyManager;
};


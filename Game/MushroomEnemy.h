#pragma once
#include "Enemy.h"

class BulletManager;

class MushroomEnemy : public Enemy
{
private:
	void InitializeModel() override;
	Vector3 CalculateHorizontalVelocity() const override;
	void Attack(float deltaTime) override;

	AnimationClip m_animationClips[2];
	float m_moveSpeed = 300.0f;
	float m_backMoveSpeed = 150.0f;
	float m_attackTimer = 0.0f;
	float m_attackInterval = 1.0f;
	float m_attackRange = 650.0f;
	BulletManager* m_bulletManager = nullptr;
};

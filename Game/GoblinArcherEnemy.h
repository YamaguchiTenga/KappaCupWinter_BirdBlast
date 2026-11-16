#pragma once
#include "Enemy.h"

class BulletManager;

class GoblinArcherEnemy : public Enemy
{
public:
	void Render(RenderContext& rc) override;

private:
	void InitializeModel() override;
	Vector3 CalculateHorizontalVelocity() const override;
	void Attack(float deltaTime) override;
	void UpdateAnimation() override;
	void UpdateBowTransform();
	void DrawAimLine(RenderContext& rc);
	Vector3 GetFirePosition() const;

	static constexpr float AttackRange = 2000.0f;
	static constexpr float AimTime = 0.7f;
	static constexpr float ShotCooldown = 1.2f;
	AnimationClip m_archerClips[2];
	ModelRender m_bowRender;
	ModelRender m_aimLineRender;
	int m_leftHandBone = -1;
	bool m_isPlayingShot = false;
	BulletManager* m_bulletManager = nullptr;
	Vector3 m_aimDirection = Vector3::Zero;
	float m_aimTimeRemaining = 0.0f;
	float m_cooldownRemaining = 0.0f;
};

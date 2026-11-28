#include "stdafx.h"
#include "GoblinArcherEnemy.h"
#include "BulletManager.h"
#include "Player.h"
#include <cmath>

void GoblinArcherEnemy::InitializeModel()
{
	m_archerClips[0].Load("Assets/modelData/Enemy/02/Goblin_Archer_Idle.tka");
	m_archerClips[0].SetLoopFlag(true);
	m_archerClips[1].Load("Assets/modelData/Enemy/02/Goblin_Archer_Shoot.tka");
	m_archerClips[1].SetLoopFlag(false);
	m_modelRender.Init("Assets/modelData/Enemy/02/Goblin_Archer.tkm", m_archerClips, 2, enModelUpAxisY);
	m_leftHandBone = m_modelRender.FindBoneID(L"hand.L");
	m_bowRender.Init("Assets/modelData/Enemy/02/Goblin_Recurve_Shortbow.tkm", nullptr, 0, enModelUpAxisY);
	m_aimLineRender.Init("Assets/modelData/Bullets/AimLine.tkm");
	m_bulletManager = FindGO<BulletManager>("bulletManager");
}

Vector3 GoblinArcherEnemy::CalculateMoveVelocity() const
{
	const float distance = GetDistanceToPlayer();
	if (distance > AttackRange && distance <= 2000.0f)
	{
		return GetDirectionToPlayer() * 220.0f;
	}
	return Vector3::Zero;
}

void GoblinArcherEnemy::Attack(float deltaTime)
{
	const Vector3 target = GetPlayer().GetPosition() + Vector3(0.0f, 50.0f, 0.0f);
	const Vector3 toTarget = target - GetFirePosition();
	const float targetDistance = toTarget.Length();
	if (GetDistanceToPlayer() > AttackRange || targetDistance <= 0.001f)
	{
		if (m_aimTimeRemaining > 0.0f)
		{
			m_modelRender.PlayAnimation(0, 0.12f);
			m_isPlayingShot = false;
		}
		m_aimTimeRemaining = 0.0f;
		return;
	}

	if (m_cooldownRemaining > 0.0f)
	{
		m_cooldownRemaining -= deltaTime;
		return;
	}

	m_aimDirection = toTarget * (1.0f / targetDistance);

	if (m_aimTimeRemaining <= 0.0f)
	{
		m_aimTimeRemaining = AimTime;
		m_modelRender.PlayAnimation(1);
		m_isPlayingShot = true;
		return;
	}

	m_aimTimeRemaining -= deltaTime;
	if (m_aimTimeRemaining <= 0.0f)
	{
		m_bulletManager->RequestFire(GetFirePosition(), m_aimDirection * 2500.0f,
			BulletTeam::Enemy, BulletType::EnemyShot);
		m_cooldownRemaining = ShotCooldown;
	}
}

void GoblinArcherEnemy::Render(RenderContext& rc)
{
	if (!IsActive())
	{
		return;
	}

	Enemy::Render(rc);

	if (!IsDying() && m_aimTimeRemaining > 0.0f)
	{
		DrawAimLine(rc);
	}

	if (m_leftHandBone < 0)
	{
		return;
	}
	UpdateBowTransform();
	m_bowRender.Draw(rc);
}

Vector3 GoblinArcherEnemy::GetFirePosition() const
{
	return GetPosition() + Vector3(0.0f, 50.0f, 0.0f);
}

void GoblinArcherEnemy::DrawAimLine(RenderContext& rc)
{
	const Vector3 center = GetFirePosition() + m_aimDirection * (AttackRange * 0.5f);

	const float horizontal = std::hypot(m_aimDirection.x, m_aimDirection.z);
	const float pitch = -std::atan2(m_aimDirection.y, horizontal);
	const float yaw = horizontal > 0.001f ? std::atan2(m_aimDirection.x, m_aimDirection.z) : 0.0f;
	Quaternion rotation;
	DirectX::XMStoreFloat4(&rotation.vec, DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, 0.0f));
	constexpr float lineWidth = 2.0f;
	m_aimLineRender.SetTRS(center, rotation, Vector3(lineWidth, lineWidth, AttackRange));
	m_aimLineRender.Update();
	m_aimLineRender.Draw(rc);
}

void GoblinArcherEnemy::UpdateBowTransform()
{
	const Matrix& handMatrix = m_modelRender.GetBone(m_leftHandBone)->GetWorldMatrix();
	Vector3 gripPosition(-3.0f, 0.0f, 0.0f);
	handMatrix.Apply(gripPosition);
	Quaternion bowRotation;
	DirectX::XMStoreFloat4(&bowRotation.vec, DirectX::XMQuaternionRotationMatrix(handMatrix));
	m_bowRender.SetTRS(gripPosition, bowRotation, Vector3::One);
	m_bowRender.Update();
}

void GoblinArcherEnemy::UpdateAnimation()
{
	if (m_isPlayingShot && !m_modelRender.IsPlayingAnimation())
	{
		m_modelRender.PlayAnimation(0);
		m_isPlayingShot = false;
	}
}
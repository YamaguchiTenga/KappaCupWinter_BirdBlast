#include "stdafx.h"
#include "MushroomEnemy.h"
#include "BulletManager.h"

void MushroomEnemy::InitializeModel()
{
	m_animationClips[0].Load("Assets/modelData/Enemy/01/Mushroom_Monster/MushroomMon_Idle.tka");
	m_animationClips[0].SetLoopFlag(true);
	m_animationClips[1].Load("Assets/modelData/Enemy/01/Mushroom_Monster/MushroomMon_Run.tka");
	m_animationClips[1].SetLoopFlag(true);
	m_modelRender.Init("Assets/modelData/Enemy/01/Mushroom_Monster/MushroomMon.tkm", m_animationClips, 2, enModelUpAxisZ);
	m_modelRender.PlayAnimation(0);
	m_bulletManager = FindGO<BulletManager>("bulletManager");
}

Vector3 MushroomEnemy::CalculateMoveVelocity() const
{
	const Vector3& toPlayer = GetDirectionToPlayer();

	Vector3 velocity = Vector3::Zero;//速度。距離によって値を変更していく

	if (GetDistanceToPlayer() >= 800.0f)
	{
		//追跡範囲外。停止
	}
	else if (GetDistanceToPlayer() >= 600.0f)
	{
		velocity = toPlayer * m_moveSpeed;//向きと速さから、移動速度を作る
	}
	else if (GetDistanceToPlayer() >= 500.0f)
	{
		//停止
	}
	else
	{
		velocity = toPlayer * -m_backMoveSpeed;//速度を後退用のものに変更
	}

	return velocity;
}

void MushroomEnemy::Attack(float deltaTime)
{
	// 攻撃する距離でなければ、今回は何もしない。
	if (GetDistanceToPlayer() >= m_attackRange)
	{
		return;
	}

	m_attackTimer += deltaTime;
	if (m_attackTimer < m_attackInterval)
	{
		return;
	}

	Vector3 forward = Vector3::AxisZ;
	m_transform.m_rotation.Apply(forward);

	Vector3 bulletSpeed = forward * 500.0f;

	Vector3 firePosition = m_transform.m_position;
	firePosition.y += 50.0f;

	m_bulletManager->RequestFire(firePosition, bulletSpeed, BulletTeam::Enemy, BulletType::EnemyShot);

	m_attackTimer = 0.0f;
}

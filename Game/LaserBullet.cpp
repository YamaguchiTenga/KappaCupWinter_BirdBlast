#include "stdafx.h"
#include "LaserBullet.h"
#include"Bullet.h"
#include"EffectManager.h"
#include "Enemy.h"
void LaserBullet::Configure()
{
	m_modelRender.Init("Assets/modelData/Bullets/PlayerSphere.tkm");

	m_collisionSize = { 100.0f, 100.0f, 100.0f };

	m_limitTime = 2.0f;
}

void LaserBullet::Move(float deltaTime)
{
	const Vector3 previousPosition = m_transform.m_position;
	Bullet::Move(deltaTime);

	Vector3 hitPosition = Vector3::Zero;
	if (Bullet::CheckStageHit(previousPosition, m_transform.m_position, hitPosition))
	{
		Deactivate();
	}
}

void LaserBullet::OnHitEnemy(Enemy* enemy)
{
	EffectManager::PlayEffect(EffectManager::Effect::LaserHit, m_transform.m_position,m_collisionSize * 0.1);
	enemy->TakeDamage(1, m_moveSpeed);//弾の進行方向に敵をノックバックさせる形にするために、速度をそのまま渡した
	Deactivate();
}
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

void LaserBullet::OnHitEnemy(Enemy* enemy)
{
	EffectManager::PlayEffect(EffectManager::Effect::LaserHit, m_transform.m_position,m_collisionSize);
	enemy->TakeDamage(1, m_moveSpeed);//弾の進行方向に敵をノックバックさせる形にするために、速度をそのまま渡した
	Deactivate();
}
#include "stdafx.h"
#include "LaserBullet.h"
#include"Bullet.h"
#include"EffectManager.h"
void LaserBullet::Configure()
{
	m_modelRender.Init("Assets/modelData/Bullets/PlayerSphere.tkm");

	m_collisionSize = { 100.0f, 100.0f, 100.0f };

	m_limitTime = 2.0f;
}

void LaserBullet::OnHitEnemy(Enemy* enemy)
{
	EffectManager::PlayEffect(EffectManager::Effect::LaserHit, m_transform.m_position,m_collisionSize);
	Bullet::OnHitEnemy(enemy);
}
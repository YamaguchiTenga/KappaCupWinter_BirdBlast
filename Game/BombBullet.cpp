#include "stdafx.h"
#include "BombBullet.h"
void BombBullet::Configure()
{
	m_modelRender.Init("Assets/modelData/Bullets/PlayerSphere.tkm");

	m_collisionSize = { 100.0f, 100.0f, 100.0f };

	m_limitTime = 2.0f;
}

void BombBullet::Move(float deltaTime)
{
	m_moveSpeed.y -= m_gravity * deltaTime;

	Bullet::Move(deltaTime);
}


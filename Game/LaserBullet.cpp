#include "stdafx.h"
#include "LaserBullet.h"
#include"Bullet.h"
void LaserBullet::Configure()
{
	m_modelRender.Init("Assets/modelData/Bullets/PlayerSphere.tkm");

	m_collisionSize = { 100.0f, 100.0f, 100.0f };

	m_moveSpeed = { 0.0f, 0.0f, 500.0f };
	m_limitTime = 2.0f;
}
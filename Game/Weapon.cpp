#include "stdafx.h"
#include "Weapon.h"

void Weapon::Update(float deltaTime)
{
	m_fireCooldown -= deltaTime;

	if (m_fireCooldown <= 0.0f)
	{
		m_fireCooldown = 0.0f;
	}
}
#include "stdafx.h"
#include "Weapon.h"
#include "AimGuide.h"

void Weapon::InitializeAimGuide()
{
	if (m_aimGuide == nullptr)
	{
		m_aimGuide = NewGO<AimGuide>(0);
	}
}

Weapon::~Weapon()
{
	if (m_aimGuide != nullptr)
	{
		DeleteGO(m_aimGuide);
	}
}
void Weapon::Update(float deltaTime)
{
	m_fireCooldown -= deltaTime;

	if (m_fireCooldown <= 0.0f)
	{
		m_fireCooldown = 0.0f;
	}
}

void Weapon::BuildStraightTrajectory(//直線の軌道で10個分の標準を作ったが、始点と終点をつなぐ形になったので今は使ってない
	const Vector3& position,
	const Vector3& direction,
	float length,
	std::vector<Vector3>& trajectoryPoints)
{
	trajectoryPoints.clear();

	for (int i = 0; i < 10; i++)
	{
		float distance = (length / 10.0f) * (i + 1);
		Vector3 aimPoint = position + direction * distance;
		trajectoryPoints.push_back(aimPoint);
	}
}
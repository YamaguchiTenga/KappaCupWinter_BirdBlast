#include "stdafx.h"
#include "AbilityController.h"

void AbilityController::StartSpeed()
{
	if (m_isOverheat)
	{
		return;
	}

	m_isPowerActive = false;

	m_isSpeedActive = true;
}

void AbilityController::StopSpeed()
{
	m_isSpeedActive = false;
}

void AbilityController::StartPower()
{
	if (m_isOverheat)
	{
		return;
	}

	m_isPowerActive = true;

	m_isSpeedActive = false;
}

void AbilityController::StopPower()
{
	m_isPowerActive = false;
}

void AbilityController::Update(float deltaTime)
{
	if (m_isSpeedActive || m_isPowerActive)
	{
		m_gauge += m_gaugeIncreseSpeed * deltaTime;

		if (m_gauge >= m_maxGauge)//上限に達したら
		{
			m_gauge = m_maxGauge;//上限以上にならないように

			m_isPowerActive = false;
			m_isSpeedActive = false;
			m_isOverheat = true;
		}
	}
	else
	{
		m_gauge -= m_gaugeRecoverySpeed * deltaTime;

		if (m_gauge <= 0.0f)
		{
			m_gauge = 0.0f;
			m_isOverheat = false;
		}
	}
}

float  AbilityController::GetSlowDeltaTime(float deltaTime) const
{
	if (m_isSpeedActive)
	{
		return deltaTime * m_slowTimeSpeed;
	}

	return deltaTime;
}
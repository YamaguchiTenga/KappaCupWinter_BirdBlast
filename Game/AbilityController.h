#pragma once
class AbilityController
{
public:
	void StartSpeed();
	void StopSpeed();

	void StartPower();
	void StopPower();

	void Update(float deltaTime);

	bool IsSpeedActive() const
	{
		return m_isSpeedActive;
	}

	bool IsPowerActive() const
	{
		return m_isPowerActive;
	}

	bool IsOverheat() const
	{
		return m_isOverheat;
	}

	float GetGauge() const
	{
		return m_gauge;
	}

	float GetSlowDeltaTime(float deltaTime) const;
	float GetSlowTimeScale() const//わかりにくいけどこっちはアニメーション用
	{
		return m_isSpeedActive ? m_slowTimeSpeed : 1.0f;
	}

private:
	bool m_isPowerActive = false;
	bool m_isSpeedActive = false;
	bool m_isOverheat = false;

	float m_maxGauge = 500.0f;
	float m_gauge = 0.0f;

	float m_gaugeIncreseSpeed = 20.0f;//1秒あたりの増加量
	float m_gaugeRecoverySpeed = 10.0f;//1秒あたりの回復量

	float m_slowTimeSpeed = 0.3f;
};


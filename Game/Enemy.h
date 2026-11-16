#pragma once
#include"Actor.h"
#include "HitStop.h"

class Player;

class Enemy : public Actor
{
public:
	Enemy();
	~Enemy();

	bool Start()override;

	void Update()override;

	void Move(float deltaTime);

	void TakeDamage(int damage, const Vector3& knockbackDirection = Vector3::Zero);

	void CheckDeath();

	void Deactivate();//停止させるという意味

	void Rotate(float deltaTime);


	virtual void UpdateAnimation();

	void UpdatePlayerInfo();//プレイヤーへの向きと距離を更新するための関数

	void Render(RenderContext& rc)override;

	//ここのconstは関数内でpositionの変更ができなくなるということ
	void SetPosition(const Vector3& position)
	{
		//position += 1;とか書いたらエラーになる

		m_transform.m_position = position;
	}

	const Vector3& GetPosition() const
	{
		return m_transform.m_position;
	}

	CharacterController& GetCharacterController()
	{
		return m_characterController;
	}

	bool IsActive() const
	{
		return m_isActive;
	}
protected:
	bool IsDying() const { return m_isDying; }
	const Player& GetPlayer() const { return *m_player; }
	virtual void InitializeModel() = 0;
	virtual Vector3 CalculateHorizontalVelocity() const = 0;
	virtual void Attack(float deltaTime) = 0;

	const Vector3& GetDirectionToPlayer() const { return m_toPlayer; }
	float GetDistanceToPlayer() const { return m_distanceToPlayer; }

private:
	void UpdateDeath(float deltaTime);
	void UpdateHitStop(float deltaTime);

	HitStop m_hitStop;
	float m_hitShakeTime = 0.0f;
	bool m_isDying = false; // 撃破演出中か
	float m_deathTimeRemaining = 0.0f; // 消えるまでの残り時間（秒）
	Vector3 m_deathVelocity = Vector3::Zero; // 撃破時に吹っ飛ぶ速度

	int m_currentAnimation = 0;
	bool m_isMoving = false;

	CharacterController m_characterController;
	int m_hp = 0;
	bool m_isActive = true;
	Vector3 m_toPlayer = Vector3::Zero; // プレイヤーへ向かう方向
	float m_distanceToPlayer = 0.0f;    // 横方向の距離
	float m_turnSpeed = 200.0f; // 1秒で旋回できる角度
	float m_verticalSpeed = 0.0f; //現在の上下方向の速度
	float m_gravity = 1500.0f;//重力
	float m_deathGravity = 2400.0f;

	Player* m_player = nullptr;
};

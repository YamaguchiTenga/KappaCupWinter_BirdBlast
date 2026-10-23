#pragma once
#include"Actor.h"

class Player;
class BulletManager;

class Enemy : public Actor
{
public:
	Enemy();
	~Enemy();

	bool Start();

	void Update();

	void Move(float deltaTime);

	void TakeDamage(int damage);

	void CheckDeath();

	void Deactivate();//停止させるという意味

	void Rotate();

	void Rotate(float deltaTime);

	void Attack();

	void UpdateAnimation();

	void UpdatePlayerInfo();//プレイヤーへの向きと距離を更新するための関数

	void Render(RenderContext& rc);

	//ここのconstは関数内でpositionの変更ができなくなるということ
	void SetPosition(const Vector3& position)
	{
		//position += 1;とか書いたらエラーになる

		m_transform.m_position = position;
	}

	CharacterController& GetCharacterController()
	{
		return m_characterController;
	}

	bool IsActive() const
	{
		return m_isActive;
	}
private:
	AnimationClip m_animationClips[2]; // 0: Idle, 1: Walk
	int m_currentAnimation = 0;
	bool m_isMoving = false;

	CharacterController m_characterController;
	int m_hp = 0;
	bool m_isActive = true;
	float m_moveSpeed = 500.0f;
	float m_backMoveSpeed = 100.0f;
	float m_attackTimer = 0.0f;
	float m_attackInterval = 1.0f;
	Vector3 m_toPlayer = Vector3::Zero; // プレイヤーへ向かう方向
	float m_distanceToPlayer = 0.0f;    // 横方向の距離
	float m_turnSpeed = 200.0f; // 1秒で旋回できる角度

	Player* m_player = nullptr;
	BulletManager* m_bulletManager = nullptr;
};

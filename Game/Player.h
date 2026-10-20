#pragma once
#include"Actor.h"

class BulletManager;
class Player : public Actor
{
public:
	Player();
	~Player();

	bool Start();

	void Update();

	void Move();

	void Jump();

	void Rotation();

	void Render(RenderContext& rc);

	void Attack();

	const Vector3& GetPosition() const
	{
		return m_transform.m_position;
	}

	CharacterController& GetCharacterController()
	{
		return m_characterController;
	}
private:
	void UpdateAnimation();
	AnimationClip m_animationClips[2];
	int m_currentAnimation = 0;

	Vector3 m_moveSpeed = Vector3::Zero;
	Vector3 m_inputMoveSpeed = Vector3::Zero;
	Vector3 m_externalMoveSpeed = Vector3::Zero;
	CharacterController m_characterController;
	float m_jumpPower = 500.0f;
	float m_verticalSpeed = 0.0f;//verticalは縦という意味
	float m_gravity = 1500.0f; 

	BulletManager* m_bulletManager = nullptr;
};


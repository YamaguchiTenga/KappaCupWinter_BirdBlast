#pragma once
#include"Actor.h"

class Player;

class Enemy : public Actor
{
public:
	Enemy();
	~Enemy();

	bool Start();

	void Update();

	void Move();

	void TakeDamage(int damage);

	void CheckDeath();

	void Deactivate();//停止させるという意味

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
	CharacterController m_characterController;
	int m_hp = 0;
	bool m_isActive = true;
	float m_moveSpeed = 50.0f;
	float m_backMoveSpeed = 30.0f;

	Player* m_player;
};

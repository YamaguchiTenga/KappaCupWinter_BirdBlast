#pragma once
#include"Actor.h"

class Enemy : public Actor
{
public:
	Enemy();
	~Enemy();

	bool Start();

	void Update();

	void Move();

	void Render(RenderContext& rc);

	//ここのconstは関数内でpositionの変更ができなくなるということ
	void SetPosition(const Vector3& position)
	{
		//position += 1;とか書いたらエラーになる

		m_transform.m_position = position;
	}

private:
	CharacterController m_characterController;
};

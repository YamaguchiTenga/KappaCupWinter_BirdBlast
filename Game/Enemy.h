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
private:
	CharacterController m_characterController;
};

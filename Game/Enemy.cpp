#include "stdafx.h"
#include "Enemy.h"
#include"Player.h"

bool Enemy::Start()
{
	m_modelRender.Init("Assets/modelData/unityChan.tkm");
	m_characterController.Init(30.0f, 30.0f, m_transform.m_position);
	m_player = FindGO<Player>("player");

	m_hp = 3;
	return true;
}

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

void Enemy::Update()
{
	if (!m_isActive)//アクティブ中だけ
	{
		return;
	}
	CheckDeath();
	Actor::Update();
	Move();
}

void Enemy::TakeDamage(int damage)
{
	if (!m_isActive)
	{
		return;
	}
	m_hp -= damage;
}

void Enemy::CheckDeath()
{
	if (m_hp <= 0)
	{
		Deactivate();
	}
}

void Enemy::Deactivate()
{
	m_isActive = false;
	//死亡した敵の体を物理ワールドから外し、通り抜けられるようにする
    m_characterController.RemoveRigidBoby();
}

void Enemy::Move()
{
	Vector3 toPlayer = m_player->GetPosition() - m_transform.m_position;
	toPlayer.y = 0.0f;
	toPlayer.Normalize();

	// 向きと速さから、移動速度を作る
	Vector3 velocity = toPlayer * m_moveSpeed;
	m_transform.m_position = m_characterController.Execute(velocity, g_gameTime->GetFrameDeltaTime());
}

void Enemy::Render(RenderContext& rc)
{
	if (!m_isActive)
	{
		return;
	}
	Actor::Render(rc);
}
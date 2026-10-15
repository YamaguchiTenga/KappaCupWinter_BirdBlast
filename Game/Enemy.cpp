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
	float distance = toPlayer.Length();//ベクトルの長さを求める
	toPlayer.Normalize();//ベクトルの長さを1にする


	Vector3 velocity = Vector3::Zero;//速度。距離によって値を変更していく

	if (distance >= 800.0f)
	{
		//追跡範囲外。停止
	}
	else if (distance >= 400.0f)
	{
		velocity = toPlayer * m_moveSpeed;//向きと速さから、移動速度を作る
	}
	else if (distance >= 200.0f)
	{
		//停止
	}
	else
	{
		velocity = toPlayer * -m_backMoveSpeed;//速度を後退用のものに変更
	}

	//最終的な速度で動かす
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
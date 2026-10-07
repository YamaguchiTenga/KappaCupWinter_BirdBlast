#include "stdafx.h"
#include "Enemy.h"

bool Enemy::Start()
{
	m_modelRender.Init("Assets/modelData/unityChan.tkm");
	m_transform.m_position = { 50.0f,0.0f,50.0f };
	m_characterController.Init(30.0f, 30.0f, m_transform.m_position);
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
	Actor::Update();
}

void Enemy::Render(RenderContext& rc)
{
	Actor::Render(rc);
}
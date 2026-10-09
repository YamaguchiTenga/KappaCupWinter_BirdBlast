#include "stdafx.h"
#include "Actor.h"

Actor::Actor()
{

}

Actor::~Actor()
{

}

bool Actor::Start()
{
	return true;
}

void Actor::Update()
{
	m_modelRender.SetTRS(m_transform.m_position, m_transform.m_rotation, m_transform.m_scale);
	m_modelRender.Update();
}

void Actor::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
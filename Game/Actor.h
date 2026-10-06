#pragma once
#include"Transform.h"
class Actor : public IGameObject
{
protected:
	Transform m_transform;
	ModelRender m_modelRender;
public:
	Actor();
	~Actor();

	virtual bool Start();
	virtual void Update();
	virtual void Render(RenderContext& rc);
};


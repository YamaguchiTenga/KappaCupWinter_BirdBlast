#include "stdafx.h"
#include "Stage.h"

bool Stage::Start()
{
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();//当たり判定可視化
	m_modelRender.Init("Assets/modelData/Floor/Forest/Forest.tkm", nullptr, 0, enModelUpAxisY);
	m_modelRender.Update();

	ModelRender collisionModel;
	collisionModel.Init("Assets/modelData/Floor/Forest/ForestCollision.tkm", nullptr, 0, enModelUpAxisY);
	collisionModel.Update();
	m_physicsStaticObject.CreateFromModel(collisionModel.GetModel(), collisionModel.GetModel().GetWorldMatrix());
	return true;
}

Stage::Stage()
{

}

Stage::~Stage()
{

}

void Stage::Update()
{
	m_modelRender.Update();
}

void Stage::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
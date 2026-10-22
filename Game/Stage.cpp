#include "stdafx.h"
#include "Stage.h"

bool Stage::Start()
{
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();//当たり判定可視化
	m_modelRender.Init("Assets/modelData/Floor/aka/aka.tkm");
	m_modelRender.SetScale(5, 1, 5);
	m_modelRender.SetPosition(0, -30.0f, 0);

	Quaternion rotation;

	// ステージを上下方向に180度反転させる。
	rotation.SetRotationDeg(Vector3::AxisY, 180.0f);
	m_modelRender.SetRotation(rotation);

	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
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
#include "stdafx.h"
#include "Bullet.h"


bool Bullet::Start()
{
	m_modelRender.Init("Assets/modelData/unityChan.tkm");

	m_collisionObject = NewGO<CollisionObject>(0);

	m_collisionSize = { 100.0f, 100.0f, 100.0f };

	m_moveSpeed = { 0.0f, 0.0f, 500.0f };

	m_collisionObject->CreateBox(
		m_transform.m_position,
		Quaternion::Identity,
		m_collisionSize
	);

	//一度作った当たり判定を残しておく
	m_collisionObject->SetIsEnableAutoDelete(false);

	//命中判定に使う。最初は待機中なので、命中判定には使わない
	m_collisionObject->SetIsEnable(false);

	m_limitTime = 2.0f;
	return true;
}


Bullet::Bullet()
{

}

Bullet::~Bullet()
{
	if (m_collisionObject != nullptr)
	{
		DeleteGO(m_collisionObject);
	}
}

void Bullet::Update()
{
	if (!m_isActive)
	{
		return;
	}

	//時間経過でTimerを増やして、limitTimeに達したらDeactivateを呼び出す
	m_activeTimer += g_gameTime->GetFrameDeltaTime();
	if (m_activeTimer >= m_limitTime)
	{
		Deactivate();
		return;
	}

	m_transform.m_position += m_moveSpeed *g_gameTime->GetFrameDeltaTime();

	Actor::Update();

	m_collisionObject->SetPosition(m_transform.m_position);

}

void Bullet::Fire(const Vector3& position, const Vector3& moveSpeed)
{
	//場所と速度の設定
	m_transform.m_position = position;
	m_moveSpeed = moveSpeed;
	m_collisionObject->SetPosition(m_transform.m_position);

	//状態をtrueにする
	m_isActive = true;
	m_collisionObject->SetIsEnable(true);//当たり判定も同時にtrueに

	//消えるまでの時間を初期値に戻す
	m_activeTimer = 0.0f;
}

void Bullet::Deactivate()
{
	//状態をfalseにする
	m_isActive = false;
	m_collisionObject->SetIsEnable(false);//当たり判定も同時にfalseに
}

void Bullet::Render(RenderContext& rc)
{
	//アクティブ中だけ動かす
	if (!m_isActive)
	{
		return;
	}
	Actor::Render(rc);
}
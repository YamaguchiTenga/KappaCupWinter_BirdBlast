#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}

Player::~Player()
{
}

bool Player::Start()
{
	m_modelRender.Init("Assets/modelData/unityChan.tkm");
	m_transform.m_scale = { 1.0f, 1.0f, 1.0f };

	m_transform.m_rotation.SetRotationDegY(90.0f);
	m_characterController.Init(25.0f, 75.0f, m_transform.m_position);
	return true;
}

void Player::Update()
{
	Move();
	Rotation();
	Actor::Update();
}

void Player::Move()
{
	Vector3 stickL = Vector3::Zero;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;

	forward.Normalize();
	right.Normalize();

	right *= stickL.x * 500.0f;
	forward *= stickL.y * 500.0f;

	//移動速度にスティックの入力量を加算する。
	m_inputMoveSpeed = right + forward;

	if (m_characterController.IsOnGround())
	{
		m_verticalSpeed = 0.0f;

		if (g_pad[0]->IsTrigger(enButtonB))
		{
			Jump();
		}
	}
	else
	{
		m_verticalSpeed -= m_gravity * g_gameTime->GetFrameDeltaTime();
	}

	m_moveSpeed = m_inputMoveSpeed + m_externalMoveSpeed;
	m_moveSpeed.y += m_verticalSpeed;
	m_transform.m_position = m_characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void Player::Rotation()
{
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_inputMoveSpeed.x) >= 0.001f || fabsf(m_inputMoveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		m_transform.m_rotation.SetRotationYFromDirectionXZ(m_inputMoveSpeed);
	}
}

void Player::Jump()
{
	m_verticalSpeed = m_jumpPower;
}

void Player::Render(RenderContext& rc)
{
	Actor::Render(rc);
}
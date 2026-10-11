#include "stdafx.h"
#include "Player.h"
#include"BulletManager.h"

Player::Player()
{
}

Player::~Player()
{
}

bool Player::Start()
{
	m_bulletManager = FindGO<BulletManager>("bulletManager");

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
	Attack();
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
		m_transform.m_rotation.SetRotationYFromDirectionXZ(m_inputMoveSpeed);
		//移動速度のX・Z成分から向きを求め、プレイヤーを移動方向へ回転させる。
		//移動速度が{ 500, 0, 100 }の場合、少しだけZ方向を向いたX方向になる
	}
}

void Player::Jump()
{
	m_verticalSpeed = m_jumpPower;
}

void Player::Attack()
{
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		Vector3 direction = Vector3::AxisZ;//正面を指す
		m_transform.m_rotation.Apply(direction);//direction をプレイヤーの回転に合わせた方向へ変える処理。

		Vector3 bulletSpeed = direction * 500.0f;//発射方向に速さを掛けて、弾の移動速度を求める。

		Vector3 firePosition = m_transform.m_position;//プレイヤーの位置をコピーし、発射位置を50だけ高くする。
		firePosition.y += 50.0f;//コピーなので、プレイヤー自身の位置は変わらない。

		m_bulletManager->RequestFire(firePosition, bulletSpeed);//Managerの処理を呼び出す。
	}
}

void Player::Render(RenderContext& rc)
{
	Actor::Render(rc);
}
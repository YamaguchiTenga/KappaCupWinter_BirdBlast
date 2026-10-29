#include "stdafx.h"
#include "Player.h"
#include"BulletManager.h"
#include"Bullet.h"

Player::Player()
{
}

Player::~Player()
{
}

bool Player::Start()
{

	m_bulletManager = FindGO<BulletManager>("bulletManager");

	m_weaponController.Initialize(m_bulletManager);

	m_animationClips[0].Load("Assets/modelData/Player/Idle_Player.tka");
	m_animationClips[0].SetLoopFlag(true);
	m_animationClips[1].Load("Assets/modelData/Player/Walk_Player.tka");
	m_animationClips[1].SetLoopFlag(true);
	m_modelRender.Init("Assets/modelData/Player/Player_Animated.tkm", m_animationClips, 2);
	m_modelRender.PlayAnimation(0);
	m_transform.m_scale = { 2.0f, 2.0f, 2.0f };

	m_transform.m_rotation.SetRotationDegY(90.0f);
	m_characterController.Init(25.0f, 75.0f, m_transform.m_position);
	return true;
}

void Player::Update()
{
	Move();
	Rotation();
	m_weaponController.Update(g_gameTime->GetFrameDeltaTime());
	UpdateAbility();
	Attack();
	UpdateAnimation();
	UpdateDebugText();//デバック用
	Actor::Update();
}

void Player::UpdateAnimation()
{
	int animation;

	// 横方向に移動する入力があるか調べる。
	if (fabsf(m_inputMoveSpeed.x) >= 0.001f ||
		fabsf(m_inputMoveSpeed.z) >= 0.001f)
	{
		animation = 1; // Walk
	}
	else
	{
		animation = 0; // Idle
	}

	// 今と違うアニメーションなら切り替える。
	if (animation != m_currentAnimation)
	{
		m_modelRender.PlayAnimation(animation, 0.15f);
		m_currentAnimation = animation;
	}
}

void Player::Move()
{
	if (m_weaponController.IsMovementLocked())
	{
		m_inputMoveSpeed = Vector3::Zero;
		return;
	}
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
	if (m_weaponController.IsMovementLocked())
	{
		return;
	}
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
	WeaponAttackContext context;

	//攻撃ボタンの状態
	context.isPressed = g_pad[0]->IsTrigger(enButtonX);
	context.isHeld = g_pad[0]->IsPress(enButtonX);

	//発射位置：プレイヤーの位置から少し上
	context.position = m_transform.m_position;
	context.position.y += 50.0f;

	//発射方向：プレイヤーが現在向いている方向
	context.direction = Vector3::AxisZ;
	m_transform.m_rotation.Apply(context.direction);

	//現在のPOWER状態
	context.isPower = m_abilityController.IsPowerActive();

	//撃つかどうかの判断と発射は、武器側へ任せる
	m_weaponController.HandleAttack(context);

	//これでPlayer → Controller → LaserWeapon → BulletManagerの流れ
}

void Player::UpdateAbility()
{
	//使用状態に応じて、ゲージを増加・冷却する。
	m_abilityController.Update(g_gameTime->GetFrameDeltaTime());

	if (g_pad[0]->IsTrigger(enButtonLB1))
	{
		if (m_abilityController.IsSpeedActive())
		{
			m_abilityController.StopSpeed();
		}
		else
		{
			m_abilityController.StartSpeed();
		}
	}

	if (g_pad[0]->IsTrigger(enButtonRB1))
	{
		if (m_abilityController.IsPowerActive())
		{
			m_abilityController.StopPower();
		}
		else
		{
			m_abilityController.StartPower();
		}
	}
}

void Player::Render(RenderContext& rc)
{
	Actor::Render(rc);
	m_debugFont.Draw(rc);
}

//デバック用
void Player::UpdateDebugText()
{
	const wchar_t* state = L"NORMAL";

	if (m_abilityController.IsOverheat())
	{
		state = L"OVERHEAT";
	}
	else if (m_abilityController.IsSpeedActive())
	{
		state = L"SPEED";
	}
	else if (m_abilityController.IsPowerActive())
	{
		state = L"POWER";
	}

	wchar_t text[256];
	swprintf_s(
		text,
		L"State: %ls\nGage: %.1f",
		state,
		m_abilityController.GetGauge()
	);

	m_debugFont.SetText(text);
}
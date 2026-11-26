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
	m_animationClips[ShootAnimation].Load("Assets/modelData/Player/Shoot_Player.tka");
	m_animationClips[ShootAnimation].SetLoopFlag(false);
	m_animationClips[BombHoldAnimation].Load("Assets/modelData/Player/BombHold_Player.tka");
	m_animationClips[BombHoldAnimation].SetLoopFlag(true);
	m_modelRender.Init("Assets/modelData/Player/Player_Animated.tkm", m_animationClips, 4);
	m_heldBombRender.Init("Assets/modelData/Bullets/QuaterniusBomb/Bomb.tkm");
	m_heldPowerBombRender.Init("Assets/modelData/Bullets/PowerBomb/PowerBomb.tkm");
	m_modelRender.PlayAnimation(0);
	m_transform.m_scale = { 2.0f, 2.0f, 2.0f };

	m_transform.m_rotation.SetRotationDegY(90.0f);
	m_transform.m_position.z = -650.0f;
	m_characterController.Init(25.0f, 75.0f, m_transform.m_position);
	return true;
}

void Player::Update()
{
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	UpdateTakingDamage(deltaTime);

	UpdateInvincibility(deltaTime);

	UpdateWeaponSelection();
	UpdateAbility();
	Move();

	const float externalSpeed = m_externalMoveSpeed.Length();
	if (externalSpeed > 0.0f)
	{
		const float remainingSpeed = externalSpeed - KnockbackDeceleration * deltaTime;
		if (remainingSpeed > 0.0f)
		{
			m_externalMoveSpeed *= remainingSpeed / externalSpeed;
		}
		else
		{
			m_externalMoveSpeed = Vector3::Zero;
		}
	}
	Rotation();
	float cooldownRate = 1.0f;
	if (m_abilityController.IsSpeedActive())
	{
		cooldownRate = 5.0f;
	}
	m_weaponController.Update(deltaTime, cooldownRate);
	Attack();
	UpdateAnimation();
	Actor::Update();
}

void Player::TakeDamage(int damage, const Vector3& knockbackDirection)
{
	if (damage <= 0 || m_invincibleTimeRemaining > 0.0f)
	{
		return;
	}

	m_hp -= damage;
	if (m_hp < 0)
	{
		m_hp = 0;
	}

	m_TakingDamage = true;
	m_TakingDamageTimer = 0.3f;
	m_isHoldingBomb = false;
	m_isHoldingPowerBomb = false;
	m_weaponController.AttackCansel();
	constexpr float invincibleDuration = 1.0f;
	m_invincibleTimeRemaining = invincibleDuration;

	Vector3 direction = knockbackDirection;
	direction.y = 0.0f;
	if (direction.Length() > 0.001f)
	{
		direction.Normalize();
		m_externalMoveSpeed = direction * KnockbackSpeed;
	}
}

void Player::UpdateInvincibility(float deltaTime)
{
	if (m_invincibleTimeRemaining > 0.0f)
	{
		m_invincibleTimeRemaining -= deltaTime;
		if (m_invincibleTimeRemaining < 0.0f)
		{
			m_invincibleTimeRemaining = 0.0f;
		}
	}
}

void Player::UpdateTakingDamage(float deltaTime)
{
	if (!m_TakingDamage)
	{
		return;
	}

	m_TakingDamageTimer -= deltaTime;

	if (m_TakingDamageTimer <= 0.0f)
	{
		m_TakingDamageTimer = 0.0f;
		m_TakingDamage = false;
	}
}

void Player::UpdateAnimation()
{
	if (!m_isHoldingBomb && m_currentAnimation == ShootAnimation && m_modelRender.IsPlayingAnimation())
	{
		return;
	}

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

	if (m_isHoldingBomb)
	{
		animation = BombHoldAnimation;
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
		m_inputMoveSpeed = Vector3::Zero;//アニメーション再生の関係で入力による移動を0にする
		if (m_externalMoveSpeed.Length() > 0.0f)
		{
			m_transform.m_position = m_characterController.Execute(
				m_externalMoveSpeed, g_gameTime->GetFrameDeltaTime());
		}
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

	constexpr float normalMoveSpeed = 500.0f;
	constexpr float boostedMoveSpeed = 650.0f;
	float moveSpeed = normalMoveSpeed;
	if (m_abilityController.IsSpeedActive())
	{
		moveSpeed = boostedMoveSpeed;
	}
	right *= stickL.x * moveSpeed;
	forward *= stickL.y * moveSpeed;

	m_inputMoveSpeed = right + forward;

	if (m_weaponController.IsMoveInputLocked())//ボムの構え中
	{
		m_inputMoveSpeed = Vector3::Zero;
	}

	if (m_characterController.IsOnGround())
	{
		m_verticalSpeed = 0.0f;

		if (g_pad[0]->IsTrigger(enButtonA))
		{
			Jump();
		}
	}
	else
	{
		m_verticalSpeed -= m_gravity * g_gameTime->GetFrameDeltaTime();
	}

	if (m_TakingDamage)
	{
		m_verticalSpeed = 0.0f;
		m_inputMoveSpeed = Vector3::Zero;
	}

	if (m_weaponController.IsWindActive())
	{
		Vector3 forward = Vector3::AxisZ;
		m_transform.m_rotation.Apply(forward);

		m_inputMoveSpeed = forward * m_windSpeed;
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

	if (m_weaponController.IsMoveInputLocked())
	{
		//このフレームで左右に回す角度を求める。
		float turnAngle = g_pad[0]->GetLStickXF()* m_aimTurnSpeed* g_gameTime->GetFrameDeltaTime();
	
		Vector3 forward = Vector3::AxisZ;
		m_transform.m_rotation.Apply(forward);//現在の正面の角度

		Quaternion rotation;
		rotation.SetRotationDegY(turnAngle);//rotationがturnAngle分回すという設定をする
		rotation.Apply(forward);//rotation分、forwardを変更

		m_transform.m_rotation.SetRotationYFromDirectionXZ(forward);//rotationを変更させる
		return;
	}

	if (m_weaponController.IsWindActive())
	{
		float turnAngle = g_pad[0]->GetLStickXF() * m_windTurnSpeed * g_gameTime->GetFrameDeltaTime();

		Vector3 forward = Vector3::AxisZ;
		m_transform.m_rotation.Apply(forward);

		Quaternion rotation;
		rotation.SetRotationDegY(turnAngle);
		rotation.Apply(forward);

		m_transform.m_rotation.SetRotationYFromDirectionXZ(forward);//rotationを変更させる
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
	constexpr float speedJumpMultiplier = 1.15f;
	m_verticalSpeed = m_jumpPower;
	if (m_abilityController.IsSpeedActive())
	{
		m_verticalSpeed *= speedJumpMultiplier;
	}
}

void Player::Attack()
{
	if (m_TakingDamage)
	{
		m_isHoldingBomb = false;
		m_isHoldingPowerBomb = false;
		return;
	}

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
	context.isSpeed = m_abilityController.IsSpeedActive();

	constexpr int BombWeaponIndex = 1;
	const bool isBombSelected = GetSelectedWeaponIndex() == BombWeaponIndex;
	m_isHoldingBomb = isBombSelected && context.isHeld;
	m_isHoldingPowerBomb = m_isHoldingBomb && context.isPower;
	if (isBombSelected)
	{
		Vector3 holdOffset(45.0f, 125.0f, 0.0f);
		m_transform.m_rotation.Apply(holdOffset);
		context.position = m_transform.m_position + holdOffset;
	}

	if (m_isHoldingBomb)
	{
		ModelRender& bombRender = m_isHoldingPowerBomb ? m_heldPowerBombRender : m_heldBombRender;
		const float scale = m_isHoldingPowerBomb ? 10.0f : 5.0f;
		bombRender.SetTRS(context.position, m_transform.m_rotation, Vector3(scale, scale, scale));
		bombRender.Update();
	}

	context.aimInput.x = g_pad[0]->GetLStickXF();
	context.aimInput.y = g_pad[0]->GetLStickYF();

	//撃つかどうかの判断と発射は、武器側へ任せる
	if (m_weaponController.HandleAttack(context) == WeaponAttackResult::Laser)
	{
		m_modelRender.PlayAnimation(ShootAnimation, 0.03f);
		m_currentAnimation = ShootAnimation;
	}

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

void Player::UpdateWeaponSelection()
{
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		m_weaponController.SelectWeapon(0);
	}
	else if (g_pad[0]->IsTrigger(enButtonDown))
	{
		m_weaponController.SelectWeapon(1);
	}
}

void Player::Render(RenderContext& rc)
{
	//blinkは点滅
	const float blinkInterval = 0.1f;
	const int blinkStep = static_cast<int>(m_invincibleTimeRemaining / blinkInterval);//切り捨てさせて常に整数として扱う
	if (blinkStep % 2 == 0)
	{
		Actor::Render(rc);
		if (m_isHoldingBomb)
		{
			if (m_isHoldingPowerBomb)
			{
				m_heldPowerBombRender.Draw(rc);
			}
			else
			{
				m_heldBombRender.Draw(rc);
			}
		}
	}
}

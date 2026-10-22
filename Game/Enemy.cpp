#include "stdafx.h"
#include "Enemy.h"
#include"Player.h"
#include"BulletManager.h"

bool Enemy::Start()
{
	m_animationClips[0].Load("Assets/animData/idle.tka");
	m_animationClips[0].SetLoopFlag(true);
	m_animationClips[1].Load("Assets/animData/run.tka");
	m_animationClips[1].SetLoopFlag(true);
	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClips, 2, enModelUpAxisY);
	m_modelRender.PlayAnimation(0);
	m_characterController.Init(30.0f, 30.0f, m_transform.m_position);
	m_player = FindGO<Player>("player");
	m_bulletManager = FindGO<BulletManager>("bulletManager");

	m_hp = 3;
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
	if (!m_isActive)//アクティブ中だけ
	{
		return;
	}

	const AbilityController& abilityController = m_player->GetAbilityController();//プレイヤーが持つabilityを取得
	float deltaTime = abilityController.GetSlowDeltaTime(g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetAnimationSpeed(abilityController.GetSlowTimeScale());//アニメーションの速度を調整する

	UpdatePlayerInfo();
	CheckDeath();
	if (!m_isActive)
	{
		return;
	}
	Move(deltaTime);
	Rotate();
	Attack();
	UpdateAnimation();
	Actor::Update();

}

void Enemy::UpdatePlayerInfo()
{
	m_toPlayer = m_player->GetPosition() - m_transform.m_position;
	m_toPlayer.y = 0.0f;

	//正規化する前に距離を保存する。
	m_distanceToPlayer = m_toPlayer.Length();

	if (m_distanceToPlayer > 0.001f)//距離があった時のみ正規化
	{
		m_toPlayer.Normalize();
	}
	else
	{
		m_toPlayer = Vector3::Zero;
	}
}

void Enemy::TakeDamage(int damage)
{
	if (!m_isActive)
	{
		return;
	}
	m_hp -= damage;
}

void Enemy::CheckDeath()
{
	if (m_hp <= 0)
	{
		Deactivate();
	}
}

void Enemy::Deactivate()
{
	m_isActive = false;
	//死亡した敵の体を物理ワールドから外し、通り抜けられるようにする
    m_characterController.RemoveRigidBoby();
}

void Enemy::Move(float deltaTime)
{
	const Vector3& toPlayer = m_toPlayer;

	Vector3 velocity = Vector3::Zero;//速度。距離によって値を変更していく

	if (m_distanceToPlayer >= 800.0f)
	{
		//追跡範囲外。停止
	}
	else if (m_distanceToPlayer >= 400.0f)
	{
		velocity = toPlayer * m_moveSpeed;//向きと速さから、移動速度を作る
	}
	else if (m_distanceToPlayer >= 200.0f)
	{
		//停止
	}
	else
	{
		velocity = toPlayer * -m_backMoveSpeed;//速度を後退用のものに変更
	}

	velocity = toPlayer * m_moveSpeed;//検証用のために常に向かってくるようにする
	//最終的な速度で動かす
	m_isMoving = fabsf(velocity.x) >= 0.001f || fabsf(velocity.z) >= 0.001f;
	m_transform.m_position = m_characterController.Execute(velocity, deltaTime);
}

void Enemy::Rotate()
{
	Vector3 toPlayer = m_player->GetPosition() - m_transform.m_position;
	toPlayer.y = 0.0f;

	m_transform.m_rotation.SetRotationYFromDirectionXZ(toPlayer);
}

void Enemy::Attack()
{
	// 攻撃する距離でなければ、今回は何もしない。
	if (m_distanceToPlayer < 200.0f ||
		m_distanceToPlayer >= 400.0f)
	{
		return;
	}

	m_attackTimer += g_gameTime->GetFrameDeltaTime();
	if (m_attackTimer < m_attackInterval)
	{
		return;
	}

	// UpdatePlayerInfo()で求めた方向を使う。
	Vector3 bulletSpeed = m_toPlayer * 500.0f;

	Vector3 firePosition = m_transform.m_position;
	firePosition.y += 50.0f;

	m_bulletManager->RequestFire(
		firePosition, bulletSpeed, BulletTeam::Enemy);

	m_attackTimer = 0.0f;
}

void Enemy::UpdateAnimation()
{
	int animation;

	if (m_isMoving)
	{
		animation = 1; // 移動中はWalk
	}
	else
	{
		animation = 0; // 停止中はIdle
	}

	// 今と違うアニメーションなら切り替える。
	if (animation != m_currentAnimation)
	{
		m_modelRender.PlayAnimation(animation, 0.15f);
		m_currentAnimation = animation;
	}
}

void Enemy::Render(RenderContext& rc)
{
	if (!m_isActive)
	{
		return;
	}
	Actor::Render(rc);
}
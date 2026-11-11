#include "stdafx.h"
#include "BombBullet.h"
#include "EnemyManager.h"//GetEnemies()`で敵一覧を取得するため。
#include "Enemy.h"//各敵の位置や有効状態を調べ、ダメージを与えるため。
#include"BombWeapon.h"//CalculateMovementを使うため
#include"EffectManager.h"
#include"SoundManager.h"

const Vector3 BombBullet::CollisionSize = { 100.0f, 100.0f, 100.0f };

BombBullet::~BombBullet()
{
	if (m_explosionCollision != nullptr)
	{
		DeleteGO(m_explosionCollision);
	}
}

bool BombBullet::Start()
{
	Bullet::Start();

	m_explosionCollision = NewGO<CollisionObject>(0);

	m_explosionCollision->CreateSphere(m_transform.m_position, Quaternion::Identity, m_explosionRadius);

	//一度作った当たり判定を使いまわすために自動削除をfalseに
	m_explosionCollision->SetIsEnableAutoDelete(false);

	//命中判定に使う。最初は待機中なので、命中判定には使わない
	m_explosionCollision->SetIsEnable(false);

	return true;
}
void BombBullet::Configure()
{
	m_modelRender.Init("Assets/modelData/Bullets/QuaterniusBomb/Bomb.tkm");
	m_powerModelRender.Init("Assets/modelData/Bullets/PowerBomb/PowerBomb.tkm");
	m_transform.m_scale = { 5.0f, 5.0f, 5.0f };

	m_collisionSize = BombBullet::CollisionSize;

	m_limitTime = 2.0f;
}

void BombBullet::Update()
{
	Bullet::Update();

	if (m_isPower && isActive())
	{
		Quaternion rotation;
		rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		m_powerModelRender.SetTRS(m_transform.m_position, rotation, m_transform.m_scale);
		m_powerModelRender.Update();
	}
}

void BombBullet::Render(RenderContext& rc)
{
	if (!m_isPower)
	{
		Bullet::Render(rc);
		return;
	}

	if (isActive())
	{
		m_powerModelRender.Draw(rc);
	}
}

void BombBullet::Move(float deltaTime)
{
	//移動前の位置を保存する。途中で地面などに当たったか調べるため
	Vector3 previousPosition = m_transform.m_position;
	//RayTestで当たった位置を受け取る
	Vector3 hitPosition = Vector3::Zero;
	
	if (m_isPower)
	{
		Bullet::Move(deltaTime);
	}
	else
	{
		//実際の移動と予測を同じものにするため、BombBWeaponのstaticの関数と変数を使って動きを計算
		//staticなのでインスタンスがなくても関数、変数(Gravity)が使える
		BombWeapon::CalculateMovement(m_transform.m_position, m_moveSpeed, BombWeapon::Gravity, deltaTime);
	}

	//通常・POWERの両方で着弾を調べる。
	if (BombWeapon::CheckStageHit(previousPosition, m_transform.m_position, hitPosition))
	{
		m_transform.m_position = hitPosition;
		Explosion();
	}
}

void BombBullet::Explosion()
{
	float explosionRadius = m_explosionRadius;
	if (m_isPower)
	{
		explosionRadius = m_powerExplosionRadius;
	}

	EffectManager::PlayEffect(EffectManager::Effect::BombExplosion,m_transform.m_position,{ explosionRadius, explosionRadius, explosionRadius });
	SoundManager::PlaySE(SoundManager::Effect::BombExplosion);

	m_explosionCollision->SetPosition(m_transform.m_position);
	m_explosionCollision->CreateSphere(m_transform.m_position, Quaternion::Identity, explosionRadius);
	m_explosionCollision->SetIsEnable(true);//当たり判定も同時にtrueに

	if (m_enemyManager != nullptr)
	{
		for (Enemy* enemy : m_enemyManager->GetEnemies())
		{
			if (!enemy->IsActive())
			{
				continue;
			}

			if (m_explosionCollision->IsHit(enemy->GetCharacterController()))
			{
				enemy->TakeDamage(m_damage);
			}

		}
	}
	m_explosionCollision->SetIsEnable(false);
	Deactivate();
}

void BombBullet::OnHitEnemy(Enemy* enemy)
{
	Explosion();
}



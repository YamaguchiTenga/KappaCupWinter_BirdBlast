#include "stdafx.h"
#include "Bullet.h"
#include "Player.h"
#include"EnemyManager.h"

bool Bullet::Start()
{
	Configure();


	m_collisionObject = NewGO<CollisionObject>(0);

	m_collisionObject->CreateBox(
		m_transform.m_position,
		Quaternion::Identity,
		m_collisionSize
	);

	//一度作った当たり判定を残しておく
	m_collisionObject->SetIsEnableAutoDelete(false);

	//命中判定に使う。最初は待機中なので、命中判定には使わない
	m_collisionObject->SetIsEnable(false);

	m_enemyManager = FindGO<EnemyManager>("enemyManager");
	m_player = FindGO<Player>("player");
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
	if (!m_isActive)//アクティブ中だけ動かす
	{
		return;
	}

	const AbilityController& abilityController = m_player->GetAbilityController();//プレイヤーが持つabilityを取得
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	if (m_team == BulletTeam::Enemy)
	{
		deltaTime = abilityController.GetSlowDeltaTime(g_gameTime->GetFrameDeltaTime());
	}

	//時間経過でTimerを増やして、limitTimeに達したらDeactivateを呼び出す
	m_activeTimer += deltaTime;
	if (m_activeTimer >= m_limitTime)
	{
		Deactivate();
		return;
	}

	Move(deltaTime);

	Actor::Update();

	m_collisionObject->SetPosition(m_transform.m_position);//位置の更新

	CheckHit();

}

void Bullet::Fire(const Vector3& position, const Vector3& moveSpeed, BulletTeam team)
{
	m_team = team;
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

void Bullet::Deactivate()//弾の状態をfalaseにする
{
	//状態をfalseにする
	m_isActive = false;
	m_collisionObject->SetIsEnable(false);//当たり判定も同時にfalseに
}

void Bullet::CheckHit()
{
	if (m_team == BulletTeam::Player)
	{
		if (m_player == nullptr) return;

		//EnemyManagerをincludeすることで間接的にEnemyクラスが使用可能になっている
		//GetEnemiesという敵一覧をenemiesに入れる
		const std::vector<Enemy*>& enemies = m_enemyManager->GetEnemies();

		auto first = enemies.begin();//先頭の要素を指すイテレータ
		while (first != enemies.end())//末尾の次に到達するまで繰り返す
		{
			//今の要素からEnemyのポインタを取り出す
			Enemy* enemy = *first;//*firstでfirstの中身が取り出せる
			first++;//次の要素へ進む

			if (!enemy->IsActive())//敵がアクティブ中だったら
			{
				continue;
			}

			//敵に当たったら
			if (m_collisionObject->IsHit(enemy->GetCharacterController()))
			{
				enemy->TakeDamage(1);
				Deactivate();
				return;
			}
		}
	}
	
		if (m_player == nullptr) return;
		else if (m_team == BulletTeam::Enemy)
		{
			if (m_collisionObject->IsHit(m_player->GetCharacterController()))
			{
				Deactivate();
				return;
			}
		}
		
	
}

void Bullet::Move(float deltaTime)
{
	m_transform.m_position += m_moveSpeed * deltaTime;
}

void Bullet::Render(RenderContext& rc)
{
	if (!m_isActive)
	{
		return;
	}
	Actor::Render(rc);
}

void Bullet::Configure()
{
	m_modelRender.Init("Assets/modelData/Bullets/PlayerSphere.tkm");

	m_collisionSize = { 100.0f, 100.0f, 100.0f };

	m_limitTime = 2.0f;
}
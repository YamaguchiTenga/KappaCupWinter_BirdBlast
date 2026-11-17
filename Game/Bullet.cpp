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
		OnLifetimeExpired();
		return;
	}

	const Vector3 previousPosition = m_transform.m_position;
	Move(deltaTime);

	if (m_isActive && m_team == BulletTeam::Enemy)
	{
		Vector3 hitPosition = Vector3::Zero;
		if (CheckStageHit(previousPosition, m_transform.m_position, hitPosition))
		{
			Deactivate();
		}
	}

	// 移動中に爆発したなどで無効になった場合は、以降の更新を行わない。
	if (!m_isActive)
	{
		return;
	}

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

			if (!enemy->IsStart() || !enemy->IsActive())//敵がアクティブ中だったら
			{
				continue;
			}

			//敵に当たったら
			if (m_collisionObject->IsHit(enemy->GetCharacterController()))
			{
				OnHitEnemy(enemy);
				return;
			}
		}
	}
	
		if (m_player == nullptr) return;
		else if (m_team == BulletTeam::Enemy)
		{
			if (m_collisionObject->IsHit(m_player->GetCharacterController()))
			{
				m_player->TakeDamage(10);
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

	m_collisionSize = { 10.0f, 10.0f, 10.0f };

	m_limitTime = 2.0f;
}

void Bullet::OnLifetimeExpired()
{
	Deactivate();
}

void Bullet::OnHitEnemy(Enemy* enemy)
{
	enemy->TakeDamage(1);
	Deactivate();
}

bool Bullet::CheckStageHit(const Vector3& start,const Vector3& end,Vector3& hitPosition)
{
	//始点と終点を、物理ライブラリで使う座標の型へ変換する。
	btVector3 rayStart(start.x, start.y, start.z);
	btVector3 rayEnd(end.x, end.y, end.z);

	//通常の判定結果の受け取り役を継承し、ステージ用に拡張する。
	struct StageRayCallback	: public btCollisionWorld::ClosestRayResultCallback//継承したのは、線を飛ばして調べた結果のうち、始点から一番近い命中を記録するクラス
	{
		//StageRayCallbackのコンストラクタ。初期化子リストで始点と終点を、親のコンストラクターに渡している。
		StageRayCallback(const btVector3& start, const btVector3& end)	: btCollisionWorld::ClosestRayResultCallback(start, end)
		{

		}

		//この物体を、線の判定対象にするか返す。
		bool needsCollision(btBroadphaseProxy* proxy) const override
		{
			//親が判定対象から外した物体は、こちらでも調べない。
			//needsCollision()は物理の衝突の基本の条件
			if (!btCollisionWorld::ClosestRayResultCallback::needsCollision(proxy))
			{
				return false;
			}

			//判定候補の物体を取り出す。情報を読むだけなのでconstにする。
			//proxyには、判定候補の物体の管理情報が入る。
			//proxy->m_clientObjectから、候補の物体へのポインターを取り出せる。
			//objectはconst btCollisionObject*型に変換されたproxy->m_clientObjectとして扱う
			const btCollisionObject* object = static_cast<const btCollisionObject*>(proxy->m_clientObject);

			//剛体ならそのポインターを取得する。剛体でなければnullptrになる。
			const btRigidBody* body = btRigidBody::upcast(object);

			//物理計算で動かさない設定の剛体（床や壁など）だけを対象にする。
			//今の速度が0かではなく、静止物体として登録されているかを調べる。
			return body != nullptr && body->isStaticObject();
		}
	};

	//線が当たった結果を受け取る。
	//複数に当たった場合は、始点に最も近い結果を保持する。
	StageRayCallback callback(rayStart, rayEnd);

	// 始点から終点まで、callbackで指定した対象への命中を調べる。
	PhysicsWorld::GetInstance()->GetDynamicWorld()->rayTest(
		rayStart,
		rayEnd,
		callback
	);

	// 何にも当たらなかった場合。
	if (!callback.hasHit())
	{
		return false;
	}

	// 当たった位置を、Vector3に変換して渡す。
	hitPosition = Vector3(
		callback.m_hitPointWorld.x(),
		callback.m_hitPointWorld.y(),
		callback.m_hitPointWorld.z()
	);

	return true;
}

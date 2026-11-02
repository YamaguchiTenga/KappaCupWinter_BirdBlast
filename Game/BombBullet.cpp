#include "stdafx.h"
#include "BombBullet.h"
#include "EnemyManager.h"//GetEnemies()`で敵一覧を取得するため。
#include "Enemy.h"//各敵の位置や有効状態を調べ、ダメージを与えるため。

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

	//一度作った当たり判定を残しておく
	m_explosionCollision->SetIsEnableAutoDelete(false);

	//命中判定に使う。最初は待機中なので、命中判定には使わない
	m_explosionCollision->SetIsEnable(false);

	return true;
}
void BombBullet::Configure()
{
	m_modelRender.Init("Assets/modelData/Bullets/PlayerSphere.tkm");

	m_collisionSize = { 100.0f, 100.0f, 100.0f };

	m_limitTime = 2.0f;
}

void BombBullet::Move(float deltaTime)
{
	m_moveSpeed.y -= m_gravity * deltaTime;

	// 移動前の位置を保存する。途中で地面などに当たったか調べるため。
	Vector3 previousPosition = m_transform.m_position;
	// RayTestで当たった位置を受け取る。
	Vector3 hitPosition = Vector3::Zero;

	Bullet::Move(deltaTime);

	if (CheckStageHit(previousPosition, m_transform.m_position, hitPosition))
	{
		//移動後の位置(第二引数)から実際に当たった位置へ戻し、そこを爆発の中心にする。
		m_transform.m_position = hitPosition;
		Explosion();
	}
}

void BombBullet::Explosion()
{
	m_explosionCollision->SetPosition(m_transform.m_position);
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

bool BombBullet::CheckStageHit(
	const Vector3& start,
	const Vector3& end,
	Vector3& hitPosition)
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
			// 親が判定対象から外した物体は、こちらでも調べない。
			//needsCollision()は物理の衝突の基本の条件			
			if (!btCollisionWorld::ClosestRayResultCallback::needsCollision(proxy))
			{
				return false;
			}

			//判定候補の物体を取り出す。情報を読むだけなのでconstにする。
			//proxyには、判定候補の物体の管理情報が入る。
			//proxy->m_clientObjectから、候補の物体へのポインターを取り出せる。
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

void BombBullet::OnHitEnemy(Enemy* enemy)
{
	Explosion();
}
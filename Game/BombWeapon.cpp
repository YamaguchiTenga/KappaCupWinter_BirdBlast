#include "stdafx.h"
#include "BombWeapon.h"
#include"BulletManager.h"
#include "AimGuide.h"
#include"BombBullet.h"
#include"Enemy.h"
#include"EnemyManager.h"


void BombWeapon::SetBulletManager(BulletManager* bulletManager)
{
    m_bulletManager = bulletManager;

    if (m_aimGuide == nullptr)
    {
        m_aimGuide = NewGO<AimGuide>(0);
    }

	m_predictionCollision = NewGO<CollisionObject>(0);
	m_predictionCollision->CreateBox(Vector3::Zero, Quaternion::Identity, BombBullet::CollisionSize);
	//一度作った当たり判定を残しておく
	m_predictionCollision->SetIsEnableAutoDelete(false);

	//初期状態では無効フラグにしておく
	m_predictionCollision->SetIsEnable(false);
}

BombWeapon::~BombWeapon()
{
    if (m_aimGuide != nullptr)
    {
        DeleteGO(m_aimGuide);
    }
	if (m_predictionCollision != nullptr)
	{
		DeleteGO(m_predictionCollision);
	}
}

void BombWeapon::HandleAttack(const WeaponAttackContext& context)
{
	//前フレームは押していて今は押していなければ、離した瞬間
	bool isReleased = m_wasAttackHeld && !context.isHeld;

	//次のフレームで比較するため今回の入力状態を保存する
	m_wasAttackHeld = context.isHeld;

	m_isLandingFound = false;

	if (context.isHeld)//構え中に速さを調整
	{
		m_bulletSpeed += context.aimInput.y * m_aimSpeedChangeRate * g_gameTime->GetFrameDeltaTime();

		if (m_bulletSpeed <= m_minBulletSpeed)
		{
			m_bulletSpeed = m_minBulletSpeed;
		}
		if (m_bulletSpeed >= m_maxBulletSpeed)
		{
			m_bulletSpeed = m_maxBulletSpeed;
		}
	}

	Vector3 bulletSpeed = CalculateVelocity(context.direction,context.isPower);//この初速は予測と発射の両方で使う

	if (context.isHeld && !context.isPower)//着弾地点を予測
	{
		m_isLandingFound = PredictLandingPosition(context.position, bulletSpeed, m_landingPosition, m_trajectoryPoints);//この返り値はtrueかfalseなので入れられる
	}
	m_aimGuide->SetTrajectoryPoints(m_trajectoryPoints);
	m_aimGuide->SetVisible(m_isLandingFound);

	if (m_isLandingFound)
	{
		m_aimGuide->SetPosition(m_landingPosition);
	}

	if (!isReleased)//射撃ボタンを離した瞬間でなければ終了(射撃ボタンを離した瞬間に下の処理に移行)
	{
		return;
	}

	if (!CanFire())//射撃が可能でなければ終了
	{
		return;
	}

	if (m_bulletManager->RequestFire(context.position, bulletSpeed, BulletTeam::Player, BulletType::Bomb, context.isPower))
	{
		StartCooldown();//発射ができた時だけクールダウンを発生させる
	}
}

//発射された瞬間の速度を計算
Vector3 BombWeapon::CalculateVelocity(const Vector3& direction, bool isPower) const
{
	Vector3 bulletSpeed = direction * m_bulletSpeed;//発射方向に速さを掛けて、弾の移動速度を求める。
	if (!isPower)
	{
		bulletSpeed.y = 800.0f;
	}
	return bulletSpeed;
}

//発射された後の移動を計算
void BombWeapon::CalculateMovement(Vector3& position, Vector3& velocity, float gravity, float deltaTime)
{
	velocity.y -= gravity * deltaTime;

	position += velocity * deltaTime;
}

//着弾地点の予測
bool BombWeapon::PredictLandingPosition(const Vector3& position, const Vector3& velocity, Vector3& landingPosition, std::vector<Vector3>& trajectoryPoints) const
{
	Vector3 predictedPosition = position;//計算中の位置
	Vector3 predictedVelocity = velocity;//計算中の速度
	float simulationDeltaTime = g_gameTime->GetFrameDeltaTime();

	trajectoryPoints.clear();

	//時間が進まない場合は繰り返しを始めない。
	if (simulationDeltaTime <= 0.0f)
	{
		return false;
	}

	float simulatedTime = 0.0f;//計算済みの時間。
	float predictionDuration = 2.0f;//予測する上限時間。今のボムの寿命に合わせた仮の値なので、staticにするのもありかも

	trajectoryPoints.push_back(predictedPosition);//発射位置を配列の最初へ追加
	while (simulatedTime < predictionDuration)
	{
		Vector3 previousPosition = predictedPosition;//移動前をコピーして保存。予測した移動区間で、ステージに当たったか調べるため

		BombWeapon::CalculateMovement(predictedPosition, predictedVelocity, BombWeapon::Gravity, simulationDeltaTime);//ここで移動して移動後の値となる

		if (BombWeapon::CheckStageHit(previousPosition, predictedPosition, landingPosition))
		{
			trajectoryPoints.push_back(landingPosition);
			return true;//着弾地点が見つかったらPredictLandingPositionにtrueを返す。このtrueを描画の判断に使う
		}

		m_predictionCollision->SetPosition(predictedPosition);//予測用の判定を今回計算した位置へ移す。

		for (Enemy* enemy : m_enemyManager->GetEnemies())
		{
			if (!enemy->IsActive())
			{
				continue;
			}

			if (m_predictionCollision->IsHit(enemy->GetCharacterController()))
			{
				landingPosition = predictedPosition;
				trajectoryPoints.push_back(landingPosition);

				return true;
			}
		}

		trajectoryPoints.push_back(predictedPosition);


		simulatedTime += simulationDeltaTime;
	}

	return false;//見つからなかったらfalse
}


bool BombWeapon::CheckStageHit(const Vector3& start,const Vector3& end,Vector3& hitPosition)
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

bool BombWeapon::IsMoveInputLocked()const
{
	return m_wasAttackHeld;
}

void BombWeapon::OnDeselected()
{
	m_wasAttackHeld = false;
	m_isLandingFound = false;
	m_aimGuide->SetVisible(false);
}
#include "stdafx.h"
#include "PowerLaserBullet.h"
#include "Enemy.h"
#include "EnemyManager.h"

PowerLaserBullet::PowerLaserBullet()
{

}

PowerLaserBullet::~PowerLaserBullet()
{
	if (m_collisionObject != nullptr)
	{
		DeleteGO(m_collisionObject);
	}
}

bool PowerLaserBullet::Start()
{
	m_modelRender.Init("Assets/modelData/Bullets/laser.tkm");
	m_enemyManager = FindGO<EnemyManager>("enemyManager");
	m_collisionObject = NewGO<CollisionObject>(0);

	m_collisionObject->CreateBox(m_transform.m_position,Quaternion::Identity,m_collisionSize);
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetIsEnable(false);
	return true;
}

void PowerLaserBullet::Update()
{
	UpdateState();

	if (!IsActive())
	{
		return;
	}
	CheckHit(g_gameTime->GetFrameDeltaTime());
	Actor::Update();
}

void PowerLaserBullet::Fire(const Vector3& position, const Vector3& direction)
{
	m_hitRecords.clear(); //新しい照射では、前回の命中記録を消す

	m_transform.m_position = position;
	m_transform.m_rotation.SetRotationYFromDirectionXZ(direction);

	Vector3 forward = Vector3::AxisZ;
	m_transform.m_rotation.Apply(forward);

	Vector3 collisionPosition = position + forward * m_collisionForwardOffset;
	m_collisionObject->SetPosition(collisionPosition);
	m_collisionObject->SetRotation(m_transform.m_rotation);
	m_collisionObject->SetIsEnable(true);

	m_remainingTime = m_fireDuration;
	m_transform.m_scale = { 1.0f, 1.0f, 1.0f };

	m_state = State::Firing;
}

void PowerLaserBullet::UpdateState()
{
	if (m_state == State::Idle)
	{
		return;
	}

	m_remainingTime -= g_gameTime->GetFrameDeltaTime();

	if (m_state == State::Firing)
	{
		if (m_remainingTime <= 0.0f)
		{
			m_state = State::Ending;
			m_collisionObject->SetIsEnable(false);
			m_remainingTime = m_endDuration;//残り時間に終了演出の時間を入れる
			// 攻撃判定の無効化は、判定を作るときにここへ追加する。
		}
	}
	else if (m_state == State::Ending)
	{
		float ratio = m_remainingTime / m_endDuration;//残り時間を、設定している終了演出の時間で割ってサイズを決める
		if (ratio < 0.0f)
		{
			ratio = 0.0f;
		}

		m_transform.m_scale.x = ratio;
		m_transform.m_scale.y = ratio;

		if (m_remainingTime <= 0.0f)
		{
			m_remainingTime = 0.0f;
			m_state = State::Idle;
		}
	}
}

void PowerLaserBullet::CheckHit(float deltaTime)
{
	if (!IsFiring() || m_collisionObject == nullptr || m_enemyManager == nullptr)
	{
		return;
	}

	// 記録済みの敵の待ち時間を減らす。
	for (HitRecord& record : m_hitRecords)
	{
		record.cooldown -= deltaTime;
	}

	for (Enemy* enemy : m_enemyManager->GetEnemies())
	{
		if (!enemy->IsActive())
		{
			continue;
		}

		//当たったら
		if (!m_collisionObject->IsHit(enemy->GetCharacterController()))
		{
			continue;
		}

		// 今調べている敵の命中記録を指すポインター。
		HitRecord* hitRecord = nullptr;

		// 記録一覧から、今の敵に対応するものを探す。
		for (HitRecord& record : m_hitRecords)
		{
			if (record.enemy == enemy)
			{
				//コピーではなく、一覧内にある記録のアドレスを覚える。
				//このポインターを通して、元の記録のcooldownを変更できる。
				hitRecord = &record;
				break;// 見つかったので検索を終える。
			}
		}

		if (hitRecord == nullptr) //今の敵の命中記録がまだない場合
		{
			//初めて当たったので、すぐにダメージを与える。
			enemy->TakeDamage(m_damage);

			//この敵と、次にダメージを与えるまでの待ち時間をセットにする。
			HitRecord record;
			record.enemy = enemy;                //今当たった敵を覚える。
			record.cooldown = m_damageInterval;  //待ち時間を設定する。

			// 作った記録を一覧へ追加する。
			m_hitRecords.push_back(record);
		}
		else if (hitRecord->cooldown <= 0.0f) //記録があり、待ち時間も終わっている場合
		{
			//同じ敵に再びダメージを与える。
			enemy->TakeDamage(m_damage);

			//一覧内の記録の待ち時間を設定し直す。
			hitRecord->cooldown = m_damageInterval;
		}
		//記録があっても待ち時間が残っていれば、今回はダメージを与えない。
	}
}
void PowerLaserBullet::Render(RenderContext& rc)
{
	if (!IsActive())
	{
		return;
	}

	Actor::Render(rc);
}
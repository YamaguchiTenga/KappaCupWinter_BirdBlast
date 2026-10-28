#pragma once
#include"Actor.h"
#include <vector>

class Enemy;
class EnemyManager;

class PowerLaserBullet : public Actor
{
public:
	PowerLaserBullet();
	~PowerLaserBullet();

	void UpdateState();

	bool Start() override;

	void Update() override;

	bool IsActive() const
	{
		return m_state != State::Idle;
	}

	bool IsFiring() const
	{
		return m_state == State::Firing;
	}

	void Fire(const Vector3& position, const Vector3& direction);

	void CheckHit(float deltaTime);

	void Render(RenderContext& rc) override;
private:
	Vector3 m_collisionSize = { 100.0f, 100.0f, 1000.0f };
	float m_collisionForwardOffset = 500.0f; //判定の中心を前方へずらす距離。プレイヤーの位置にそのまま置くと後ろにも伸びてしまうので、仮の処置。

	enum class State
	{
		Idle, // 待機中
		Firing,   // 照射中：攻撃判定あり
		Ending,   // 終了演出中：攻撃判定なし
	};
	State m_state = State::Idle;

	float m_remainingTime = 0.0f;//照射終了までの残り時間（秒）
	float m_fireDuration = 1.0f;//照射する時間（秒）
	float m_endDuration = 0.1f; //終了演出の時間（秒）


	//敵ごとの、次にダメージを与えられるまでの時間
	struct HitRecord
	{
		Enemy* enemy = nullptr;
		float cooldown = 0.0f;
	};

	std::vector<HitRecord> m_hitRecords;//要素数を増減できる配列。敵一覧としている

	float m_damageInterval = 0.1f; //継続ダメージの間隔（秒）
	int m_damage = 1;              //1回あたりのダメージ

	CollisionObject* m_collisionObject = nullptr;
	EnemyManager* m_enemyManager = nullptr;

};


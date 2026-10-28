#pragma once
#include "Actor.h"

class EnemyManager;
class Player;

enum class BulletTeam
{
	Player,
	Enemy,
};

enum BulletType
{
	Laser,     // プレイヤーの基本射撃
	Bomb,      // ボム
	EnemyShot, // 敵の基本射撃
};

class Bullet : public Actor
{
public:
	Bullet();
	~Bullet();

	bool Start();

	void Update();

	void Render(RenderContext& rc);

	void Fire(const Vector3& position, const Vector3& moveSpeed,BulletTeam team);

	void Deactivate();//停止させるという意味

	void CheckHit();

	bool isActive()const
	{
		return m_isActive;
	}

protected:
	virtual void Move(float deltaTime);

	//弾の見た目・当たり判定の大きさ・寿命を設定する。
	//子クラスで弾の種類ごとの設定を行う。
	virtual void Configure();

	float m_limitTime = 0.0f;
	Vector3 m_moveSpeed = Vector3::Zero;
	Vector3 m_collisionSize = Vector3::Zero;

	BulletTeam m_team = BulletTeam::Player;//敵の弾かプレイヤーの弾か判断する用
private:
	CollisionObject* m_collisionObject = nullptr;
	bool m_isActive = false;
	float m_activeTimer = 0.0f;//発射して、何秒経過したか記録する

	EnemyManager* m_enemyManager = nullptr;
	Player* m_player = nullptr;
};


#pragma once
#include"Weapon.h"
#include <vector>

class Enemy;
class EnemyManager;
class WindVisual;

class WindWeapon : public Weapon
{
public:
	~WindWeapon() override;
	void Initialize(EnemyManager* enemyManager);
	WeaponAttackResult HandleAttack(const WeaponAttackContext& context) override;
	void CheckHit(float deltatime, const WeaponAttackContext& context);
	void PullEnemies(const WeaponAttackContext& context);
	void AttackCansel() override;
	void OnDeselected() override;
	bool IsWindActive() const override
	{
		return m_isActive;
	}
private:
	// AI実装の見た目専用オブジェクト。生成と表示切替だけをこの武器から行う。
	WindVisual* m_visual = nullptr;
	//前フレームの情報を覚えるためメンバに
    //敵ごとの、次にダメージを与えられるまでの時間
    struct HitRecord
    {
        Enemy* enemy = nullptr;
        float cooldown = 0.0f;
    };
    std::vector<HitRecord> m_hitRecords;//要素数を増減できる配列。敵一覧としている

	bool m_isActive = false;
	
	EnemyManager* m_enemyManager = nullptr;
	CollisionObject* m_collisionObject = nullptr;
	float m_collisionRadius = 140.0f;
	float m_pullRadius = 500.0f;
};


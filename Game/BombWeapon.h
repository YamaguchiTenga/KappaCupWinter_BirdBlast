#pragma once
#include"Weapon.h"
#include<vector>

class BulletManager;
class EnemyManager;

class BombWeapon : public Weapon
{
public:
	void HandleAttack(const WeaponAttackContext& context)override;
	bool IsMoveInputLocked() const override;
	~BombWeapon() override;
	void OnDeselected() override;
public:
	void SetBulletManager(BulletManager* bulletManager);

	void SetEnemyManager(EnemyManager* enemyManager)//controllerから受けとる
	{
		m_enemyManager = enemyManager;
	}

public:
	//指定した時間ぶん位置と速度を更新する。
	static void CalculateMovement(
		Vector3& position,
		Vector3& velocity,
		float gravity,
		float deltaTime
	);

	//実際の移動と着弾予測で共通して使う重力の強さ。
	//static constexprなら、h内で値まで定義でき、cpp側の定義がいらなくなる
	static constexpr float Gravity = 3000.0f;

	//移動経路が地面や壁に当たったか調べる。予測と、実弾の両方で使う
	static bool CheckStageHit(
		const Vector3& start,
		const Vector3& end,
		Vector3& hitPosition
	);

private:
	// 実際の発射と着弾予測で共通して使う初速を作る。
	Vector3 CalculateVelocity(const Vector3& direction,bool isPower) const;

	bool PredictLandingPosition(
		const Vector3& position,
		const Vector3& velocity,
		Vector3& landingPosition,//予測した着弾地点
		std::vector<Vector3>& trajectoryPoints
	) const;

	BulletManager* m_bulletManager = nullptr;
	EnemyManager* m_enemyManager = nullptr;

	float m_bulletSpeed = 1000.0f;//移動速度

	float m_minBulletSpeed = 100.0f;//速さの下限
	float m_maxBulletSpeed = 1000.0f;//上限
	float m_aimSpeedChangeRate = 1000.0f;//スティックを最大まで倒したとき、1秒で速さをどれだけ変えるか。
private:
	//着弾予測が成功したか。成功した場合だけマーカーを表示する。
	bool m_isLandingFound = false;
	//予測した着弾位置。
	Vector3 m_landingPosition = Vector3::Zero;
	//本物のボムと同じ大きさで予測位置と敵の接触を調べる。
	CollisionObject* m_predictionCollision = nullptr;

	std::vector<Vector3> m_trajectoryPoints;//Trajectoryは軌跡
};

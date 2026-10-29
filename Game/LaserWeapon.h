#pragma once
#include"Weapon.h"

class BulletManager;

class LaserWeapon : public Weapon
{
public:
	LaserWeapon();

	virtual void HandleAttack(const WeaponAttackContext& context) override;

	bool IsMovementLocked() const override;

	// 外からBulletManagerのポインターを受け取り、発射を依頼する相手として保持する。
	//IGameObjectを継承してなくてStartは自動で呼ばれないため、Controller経由でManagerを受け取る用
	void SetBulletManager(BulletManager* bulletManager)
	{
		m_bulletManager = bulletManager;
	}

private:
	BulletManager* m_bulletManager = nullptr;

	float m_bulletSpeed = 1000.0f;//移動速度
};


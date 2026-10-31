#pragma once
#include"Weapon.h"

class BulletManager;

class BombWeapon : public Weapon
{
public:
	void HandleAttack(const WeaponAttackContext& context)override;
	//bool IsMovementLocked() const override;

public:
	void SetBulletManager(BulletManager* bulletManager)
	{
		m_bulletManager = bulletManager;
	}
private:
	BulletManager* m_bulletManager = nullptr;

	float m_bulletSpeed = 1000.0f;//移動速度
};

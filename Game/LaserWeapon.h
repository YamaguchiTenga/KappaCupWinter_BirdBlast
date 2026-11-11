#pragma once
#include"Weapon.h"

class BulletManager;

class LaserWeapon : public Weapon
{
public:
	LaserWeapon();

	void HandleAttack(const WeaponAttackContext& context) override;
	void OnDeselected() override;
	bool IsMovementLocked() const override;
	bool IsMoveInputLocked() const override;
public:
	void SetBulletManager(BulletManager* bulletManager);

private:
	BulletManager* m_bulletManager = nullptr;

	float m_bulletSpeed = 1000.0f;//移動速度
};


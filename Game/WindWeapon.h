#pragma once
#include"Weapon.h"
class WindWeapon : public Weapon
{
public:
	WeaponAttackResult HandleAttack(const WeaponAttackContext& context) override;
	void AttackCansel() override;
private:
	bool m_isActive = false;
};


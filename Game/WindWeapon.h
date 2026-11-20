#pragma once
#include"Weapon.h"
class WindWeapon : public Weapon
{
public:
	WeaponAttackResult HandleAttack(const WeaponAttackContext& context) override;
	void AttackCansel() override;

	bool IsWindActive() const override
	{
		return m_isActive;
	}
private:
	bool m_isActive = false;
};


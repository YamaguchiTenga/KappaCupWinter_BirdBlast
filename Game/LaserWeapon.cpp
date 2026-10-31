#include "stdafx.h"
#include "LaserWeapon.h"
#include"BulletManager.h"

LaserWeapon::LaserWeapon()
{
	SetFireInterval(0.5f);
}

void LaserWeapon::HandleAttack(const WeaponAttackContext& context)
{
	if (!context.isPressed)//射撃ボタンを押した瞬間でなければ終了
	{
		return;
	}

	if (!CanFire())//射撃が可能でなければ終了
	{
		return;
	}

	if (context.isPower)
	{
		if (m_bulletManager->RequestPowerLaser(context.position, context.direction))
		{
			StartCooldown();
		}

		return;
	}
	Vector3 bulletSpeed = context.direction * m_bulletSpeed;//発射方向に速さを掛けて、弾の移動速度を求める。

	if (m_bulletManager->RequestFire(context.position, bulletSpeed, BulletTeam::Player,BulletType::Laser))
	{
		StartCooldown();//発射ができた時だけクールダウンを発生させる
	}
}

bool LaserWeapon::IsMovementLocked() const
{
	return m_bulletManager->IsPowerLaserFiring();
}
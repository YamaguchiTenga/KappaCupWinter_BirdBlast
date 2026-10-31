#include "stdafx.h"
#include "BombWeapon.h"
#include"BulletManager.h"

void BombWeapon::HandleAttack(const WeaponAttackContext& context)
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
		return;
	}
	Vector3 bulletSpeed = context.direction * m_bulletSpeed;//発射方向に速さを掛けて、弾の移動速度を求める。
	bulletSpeed.y = 500.0f;

	if (m_bulletManager->RequestFire(context.position, bulletSpeed, BulletTeam::Player, BulletType::Bomb))
	{
		StartCooldown();//発射ができた時だけクールダウンを発生させる
	}
}

//bool BombWeapon::IsMovementLocked() const
//{
//	return m_bulletManager->IsPowerLaserFiring();
//}
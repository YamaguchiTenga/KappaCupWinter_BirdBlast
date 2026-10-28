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

	Vector3 bulletSpeed = context.direction * m_bulletSpeed;//発射方向に速さを掛けて、弾の移動速度を求める。

	// TODO: POWER時の仕様が決まり次第、context.isPowerに応じて弾の性能を切り替える。
	//現時点では通常時の性能で発射する。
	if (m_bulletManager->RequestFire(context.position, bulletSpeed, BulletTeam::Player,BulletType::Laser))
	{
		StartCooldown();//発射ができた時だけクールダウンを発生させる
	}
}
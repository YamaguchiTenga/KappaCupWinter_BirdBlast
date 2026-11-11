#include "stdafx.h"
#include "LaserWeapon.h"
#include"BulletManager.h"
#include "SoundManager.h"
#include"AimGuide.h"

LaserWeapon::LaserWeapon()
{
	SetFireInterval(0.5f);
	InitializeAimGuide();
}

//外(Contoroller)から呼び出して,BulletManagerのポインターを受け取り、発射を依頼する相手として保持する。
//IGameObjectを継承してなくてStartは自動で呼ばれないため、FindGOではなくこのやり方に
void LaserWeapon::SetBulletManager(BulletManager* bulletManager)
{
	m_bulletManager = bulletManager;
}

void LaserWeapon::HandleAttack(const WeaponAttackContext& context)
{
	//前フレームは押していて今は押していなければ、離した瞬間
	bool isReleased = m_wasAttackHeld && !context.isHeld;

	//次のフレームで比較するため今回の入力状態を保存する
	m_wasAttackHeld = context.isHeld;

	if (context.isHeld)
	{
		const Vector3 lineEnd = context.position + context.direction * 1000.0f;//1000は仮の数値
		m_aimGuide->SetLine(context.position, lineEnd);
	}

	m_aimGuide->SetVisible(context.isHeld);
	if (!isReleased)//離した瞬間でなければ終了
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
			SoundManager::PlaySE(SoundManager::Effect::PowerLaser);
			StartCooldown();
		}
		return;
	}

	Vector3 bulletSpeed = context.direction * m_bulletSpeed;//発射方向に速さを掛けて、弾の移動速度を求める。

	if (m_bulletManager->RequestFire(context.position, bulletSpeed, BulletTeam::Player,BulletType::Laser))
	{
		SoundManager::PlaySE(SoundManager::Effect::Laser);
		StartCooldown();//発射ができた時だけクールダウンを発生させる
	}
}

bool LaserWeapon::IsMovementLocked() const
{
	return m_bulletManager->IsPowerLaserFiring();
}

bool LaserWeapon::IsMoveInputLocked()const
{
	return m_wasAttackHeld;
}

void LaserWeapon::OnDeselected()
{
	m_wasAttackHeld = false;
	m_aimGuide->SetVisible(false);
}
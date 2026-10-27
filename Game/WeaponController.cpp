#include "stdafx.h"
#include <utility>//std::moveを使うためのもの
#include "WeaponController.h"
#include "LaserWeapon.h"
void WeaponController::Initialize(BulletManager* bulletManager)
{
	//レーザーを作る
	auto laserWeapon = std::make_unique<LaserWeapon>();//autoでstd::unique_ptr<LaserWeapon>型を省略している

	//発射を依頼するManagerを設定する。
	laserWeapon->SetBulletManager(bulletManager);

	//配列の0番へ所有権を移す。管理と削除の責任ごと渡す
	m_weapons[0] = std::move(laserWeapon);
}

void WeaponController::Update(float deltaTime)
{
	for (auto& weapon : m_weapons)//ここのautoはstd::unique_ptr<Weapon>への参照
	{
		if (weapon)//中身がnullptrじゃなければ
		{
			weapon->Update(deltaTime);//WeaponのUpdateを読んで、全ての武器共通の処理を行う
		}
	}
}	

void WeaponController::HandleAttack(const WeaponAttackContext& context)
{
	auto& weapon = m_weapons[m_selectedWeaponIndex];//選択番号に対応する配列の要素を参照する。

	if (weapon)
	{
		weapon->HandleAttack(context);
	}
}
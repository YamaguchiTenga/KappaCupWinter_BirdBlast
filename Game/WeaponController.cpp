#include "stdafx.h"
#include <utility>//std::moveを使うためのもの
#include "WeaponController.h"

// 各武器の実物を生成するため、派生クラスの定義が必要。
// h側ではWeaponの配列で所有するため、各派生クラスの前方宣言は不要。
#include "LaserWeapon.h"
#include"BombWeapon.h"

void WeaponController::Initialize(BulletManager* bulletManager)
{
	//レーザーを作る
	auto laserWeapon = std::make_unique<LaserWeapon>();//autoでstd::unique_ptr<LaserWeapon>型を省略している

	//PlayerのStartからInitializeへ渡されたBulletManagerのポインターを、
	//武器へ渡し、発射の依頼先として覚えさせる。
	laserWeapon->SetBulletManager(bulletManager);

	//配列の0番へ所有権を移す。管理と削除の責任ごと渡す
	m_weapons[0] = std::move(laserWeapon);

	//ボム
	auto bombWeapon = std::make_unique<BombWeapon>();

	bombWeapon->SetBulletManager(bulletManager);

	m_weapons[1] = std::move(bombWeapon);
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
		weapon->HandleAttack(context);//子クラスに情報を渡して発射させる
	}
}


bool WeaponController::IsMovementLocked() const
{
	for (const auto& weapon : m_weapons)
	{
		if (weapon && weapon->IsMovementLocked())
		{
			return true; // どれか1つでも制限中なら、playerから移動を制限する。
		}
	}

	return false;
}

bool WeaponController::IsMoveInputLocked() const
{
	const auto& weapon = m_weapons[m_selectedWeaponIndex];

	if (weapon)
	{
		return weapon->IsMoveInputLocked();
	}

	return false;
}

void WeaponController::SelectWeapon(int index)
{
	const auto& weapon = m_weapons[m_selectedWeaponIndex];

	if (!m_weapons[index])//この枠には武器が登録されていない
	{
		return; 
	}
	if (index == m_selectedWeaponIndex)//同じ武器なら切り替えない
	{
		return;
	}

	weapon->OnDeselected();
	m_selectedWeaponIndex = index;
}
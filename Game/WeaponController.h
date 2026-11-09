#pragma once

//各武器を所有し、選択と攻撃指示を管理する。
//非選択中も含め、全武器の発射待ち時間を更新する。

#include <array>
#include <memory>
#include "Weapon.h"

class BulletManager;

class WeaponController
{
public:
	void Initialize(BulletManager* bulletManager);//Initializeは初期化させるという意味

	void Update(float deltaTime);

	void HandleAttack(const WeaponAttackContext& context);//WeaponAttackContextはWeapon.hの上の方にある構造体

	void SelectWeapon(int index);

	bool IsMovementLocked() const;
	bool IsMoveInputLocked() const;

	int GetSelectedWeaponIndex() const
	{
		return m_selectedWeaponIndex;
	}
private:
	static const int WeaponCount = 4;//武器配列の要素数

	//武器をWeaponCount個まで所有できて、Controllerが破棄されると武器も自動で削除される配列
	// LaserWeaponやBombWeaponを、共通のWeaponとして所有・管理する。
	// 各武器の保存場所はこの配列なので、武器ごとのメンバ変数は不要。
	std::array<std::unique_ptr<Weapon>, WeaponCount> m_weapons{};

	int m_selectedWeaponIndex = 0;//選択中の武器の番号
};


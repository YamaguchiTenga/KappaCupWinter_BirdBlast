#include "stdafx.h"
#include "WindWeapon.h"

WeaponAttackResult WindWeapon::HandleAttack(const WeaponAttackContext& context)
{
    if (!context.isHeld)
    {
        AttackCansel();
        return WeaponAttackResult::None;
    }
    else
    {
        m_isActive = true;
    }
    //ここに、風を発動する処理を追加していく。

    return WeaponAttackResult::None;
}

void WindWeapon::AttackCansel()
{
    m_isActive = false;
}
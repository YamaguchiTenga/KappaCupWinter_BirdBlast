#include "stdafx.h"
#include "WeaponWheelUI.h"
#include "Player.h"
#include<cmath>//スティックの倒した量の絶対値を見るため
#include"SoundManager.h"
bool WeaponWheelUI::Start()
{
	m_player = FindGO<Player>("player");

	//アイコンごとの位置を設定する
	const Vector3 weaponIconOffsets[m_weaponCount] = {
	{ 0.0f, 350.0f, 0.0f },
	{ 0.0f, -350.0f, 0.0f },
	};

	m_ringRender.Init("Assets/UI/WeaponWheelUI/WeaponWheel_Ring.DDS", 1000.0f, 1000.0f);
	m_ringRender.SetPosition(m_position);
	m_ringRender.Update();

	m_backgroundRender.Init("Assets/UI/WeaponWheelUI/WeaponWheel_Dim.DDS", 3000.0f, 3000.0f);
	m_backgroundRender.SetPosition(m_position);
	m_backgroundRender.Update();

	m_weapon[0].Init("Assets/UI/WeaponIcons/LaserIcon.DDS", m_weaponIconSize.x, m_weaponIconSize.y);//レーザー
	m_weapon[1].Init("Assets/UI/WeaponIcons/BombIcon.DDS", m_weaponIconSize.x, m_weaponIconSize.y);//ボム

	m_selectedWeapon[0].Init("Assets/UI/WeaponIcons/LaserIcon_Selected.DDS", m_weaponIconSize.x, m_weaponIconSize.y);
	m_selectedWeapon[1].Init("Assets/UI/WeaponIcons/BombIcon_Selected.DDS", m_weaponIconSize.x, m_weaponIconSize.y);

	for (int i = 0; i < m_weaponCount; i++)
	{
		m_weapon[i].SetPosition(m_position + weaponIconOffsets[i]);
		m_selectedWeapon[i].SetPosition(m_position + weaponIconOffsets[i]);

		m_selectedWeapon[i].SetScale({ 1.3f, 1.3f, 1.0f });//選択中の武器は少し大きくして強調

		m_weapon[i].Update();
		m_selectedWeapon[i].Update();
	}

	return true;
}

void WeaponWheelUI::Render(RenderContext& rc)
{
	if (!IsVisible())
	{
		return;
	}
	m_backgroundRender.Draw(rc);
	m_ringRender.Draw(rc);
	for (int i = 0; i < m_weaponCount; i++)
	{
		//選択中なら色付きが表示されて
		if (i == m_highlightedWeaponIndex)
		{
			m_selectedWeapon[i].Draw(rc);
		}
		else//それ以外は色なし
		{
			m_weapon[i].Draw(rc);
		}
	}
}

//武器選択を開いたとき、強調される武器を選んでる武器にする
void WeaponWheelUI::InitSelectionWeapon()
{
	m_highlightedWeaponIndex = m_player->GetSelectedWeaponIndex();
}

void WeaponWheelUI::UpdateSelection(float stickX, float stickY)
{
	//スティックの倒した量のしきい値
	const float threshold = 0.5f;

	//横と縦に倒した量の絶対値を見る
	if (std::abs(stickX) > std::abs(stickY))
	{
		return; //左右の枠はまだ空いている
	}

	if (stickY >= threshold)//上に量が多い
	{
		if (0 != m_highlightedWeaponIndex)
		{
			SoundManager::PlaySE(SoundManager::Effect::WeaponSelected);
		}
		m_highlightedWeaponIndex = 0; //レーザー
	}
	else if (stickY <= -threshold)
	{
		if (1 != m_highlightedWeaponIndex)
		{
			SoundManager::PlaySE(SoundManager::Effect::WeaponSelected);
		}
		m_highlightedWeaponIndex = 1; //下のボム
	}
}
#pragma once
#include"UIBase.h"

class Player;

class WeaponWheelUI : public UIBase
{
public:
	bool Start() override;
	//void Update() override;
	void Render(RenderContext& rc)override;

	void InitSelectionWeapon();

	void UpdateSelection(float stickX, float stickY);

	//選択した武器を返す
	int GetHighlightedWeaponIndex()
	{
		return m_highlightedWeaponIndex;
	}
private:
	const Player* m_player = nullptr;
	static constexpr int m_weaponCount = 2;//配列のサイズに使うにはconstではなくこの書き方
	Vector2 m_weaponIconSize{ 150.0f,150.0f };
	SpriteRender m_ringRender;
	SpriteRender m_weapon[m_weaponCount];
	SpriteRender m_selectedWeapon[m_weaponCount];
	SpriteRender m_backgroundRender;//背景用

	int m_highlightedWeaponIndex = 0;
};


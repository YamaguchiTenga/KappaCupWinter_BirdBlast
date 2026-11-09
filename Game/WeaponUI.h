#pragma once
#include "UIBase.h"

class Player;

class WeaponUI : public UIBase
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

private:
    static const int LaserIndex = 0;
    static const int BombIndex = 1;

    Player* m_player = nullptr;
    int m_selectedWeaponIndex = LaserIndex; // 文字色とアイコンで共通の選択番号を使う。

    FontRender m_laserFont; //十字キー上のレーザー表示。
    FontRender m_bombFont;  //十字キー下のボム表示。

    SpriteRender m_laserIconRender; 
    SpriteRender m_bombIconRender;  
    SpriteRender m_laserSelectedIconRender; // 選択中のレーザー用。
    SpriteRender m_bombSelectedIconRender;  // 選択中のボム用。
};

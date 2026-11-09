#include "stdafx.h"
#include "WeaponUI.h"
#include "Player.h"

bool WeaponUI::Start()
{
    m_player = FindGO<Player>("player");

    // 左上を基準に、十字キーの上下と同じ順に文字を並べる。
    const Vector3 laserRowPosition = m_position;
    const Vector3 bombRowPosition = m_position + Vector3(0.0f, -44.0f, 0.0f);
    const Vector3 textOffset{ 42.0f, 0.0f, 0.0f };
    const Vector3 iconOffset{ 16.0f, -20.0f, 0.0f };

    m_laserFont.SetText(L"[上] レーザー");
    m_laserFont.SetPivot({ 0.0f, 1.0f });
    m_laserFont.SetPosition(laserRowPosition + textOffset);

    m_bombFont.SetText(L"[下] ボム");
    m_bombFont.SetPivot({ 0.0f, 1.0f });
    m_bombFont.SetPosition(bombRowPosition + textOffset);

    // 文字の横に並べる白版アイコンを読み込む。
    m_laserIconRender.Init("Assets/UI/WeaponIcons/LaserIcon.DDS", 32.0f, 32.0f);
    m_bombIconRender.Init("Assets/UI/WeaponIcons/BombIcon.DDS", 32.0f, 32.0f);

    // アイコンの中心を各行の左側に置き、位置を描画へ反映する。
    m_laserIconRender.SetPosition(laserRowPosition + iconOffset);
    m_bombIconRender.SetPosition(bombRowPosition + iconOffset);
    m_laserIconRender.Update();
    m_bombIconRender.Update();

    // 切り替えても位置が動かないよう、カラー版を白版と同じ位置とサイズにする。
    m_laserSelectedIconRender.Init("Assets/UI/WeaponIcons/LaserIcon_Selected.DDS", 32.0f, 32.0f);
    m_bombSelectedIconRender.Init("Assets/UI/WeaponIcons/BombIcon_Selected.DDS", 32.0f, 32.0f);
    m_laserSelectedIconRender.SetPosition(m_laserIconRender.GetPosition());
    m_bombSelectedIconRender.SetPosition(m_bombIconRender.GetPosition());
    m_laserSelectedIconRender.Update();
    m_bombSelectedIconRender.Update();

    return true;
}

void WeaponUI::Update()
{
    if (m_player == nullptr)
    {
        return;
    }

    // 描画でも同じ選択番号を使えるように覚えておく。
    m_selectedWeaponIndex = m_player->GetSelectedWeaponIndex();
    const Vector4 selectedColor{ 1.0f, 0.85f, 0.2f, 1.0f };
    const Vector4 normalColor{ 0.7f, 0.7f, 0.7f, 1.0f };

    m_laserFont.SetColor(m_selectedWeaponIndex == LaserIndex ? selectedColor : normalColor);
    m_bombFont.SetColor(m_selectedWeaponIndex == BombIndex ? selectedColor : normalColor);
}

void WeaponUI::Render(RenderContext& rc)
{
    if (!IsVisible())
    {
        return;
    }

    //選択状態に合わせて片方だけ描く。
    if (m_selectedWeaponIndex == LaserIndex)
    {
        m_laserSelectedIconRender.Draw(rc);
    }
    else
    {
        m_laserIconRender.Draw(rc);
    }

    if (m_selectedWeaponIndex == BombIndex)
    {
        m_bombSelectedIconRender.Draw(rc);
    }
    else
    {
        m_bombIconRender.Draw(rc);
    }

    m_laserFont.Draw(rc);
    m_bombFont.Draw(rc);
}

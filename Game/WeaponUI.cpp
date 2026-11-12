#include "stdafx.h"
#include "WeaponUI.h"
#include "Player.h"

bool WeaponUI::Start()
{
    m_player = FindGO<Player>("player");

    const Vector3 laserPosition = m_position;
    const Vector3 bombPosition = m_position + Vector3(132.0f, 0.0f, 0.0f);
    m_laserIconRender.Init("Assets/UI/WeaponIcons/LaserIcon.DDS", 96.0f, 96.0f);
    m_bombIconRender.Init("Assets/UI/WeaponIcons/BombIcon.DDS", 96.0f, 96.0f);

    m_laserIconRender.SetPosition(laserPosition);
    m_bombIconRender.SetPosition(bombPosition);
    m_laserIconRender.Update();
    m_bombIconRender.Update();

    // 切り替えても位置が動かないよう、カラー版を白版と同じ位置とサイズにする。
    m_laserSelectedIconRender.Init("Assets/UI/WeaponIcons/LaserIcon_Selected.DDS", 96.0f, 96.0f);
    m_bombSelectedIconRender.Init("Assets/UI/WeaponIcons/BombIcon_Selected.DDS", 96.0f, 96.0f);
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
}

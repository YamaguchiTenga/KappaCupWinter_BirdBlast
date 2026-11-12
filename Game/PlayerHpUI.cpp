#include "stdafx.h"
#include "PlayerHpUI.h"
#include "Player.h"

bool PlayerHpUI::Start()
{
    m_player = FindGO<Player>("player");

    // 背景の内側に残量バーを重ねるため、元画像の寸法で読み込む。
    m_backgroundRender.Init("Assets/UI/PlayerHpBar/HpBar_Background.DDS", 392.0f, 40.0f);
    m_fillRender.Init("Assets/UI/PlayerHpBar/HpBar_Fill.DDS", 384.0f, 32.0f);

    //左上を基準にして、横幅を変えてもバーの左端が動かないようにする。
    m_backgroundRender.SetPivot({ 0.0f, 1.0f });
    m_fillRender.SetPivot({ 0.0f, 1.0f });

    //基準位置に背景を置き、その内側に緑のバーを重ねる。
    m_backgroundRender.SetPosition(m_position);
    m_fillRender.SetPosition(m_position + Vector3(4.0f, -4.0f, 0.0f));
    m_backgroundRender.Update();
    m_fillRender.Update();
    m_hpText.SetPivot(1.0f, 0.5f);
    m_hpText.SetPosition(m_position + Vector3(120.0f, -5.0f, 0.0f));
    m_hpText.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_hpText.SetScale(0.8f);
    Update();
    return true;
}

void PlayerHpUI::Update()
{
    if (m_player == nullptr)
    {
        return;
    }

    // HPの割合を小数で求め、バーの横幅に反映する。
    const int hp = m_player->GetHp();
    const int maxHp = m_player->GetMaxHp();
    wchar_t hpText[32];
    swprintf_s(hpText, L"%d / %d", hp, maxHp);
    m_hpText.SetText(hpText);

    const float hpRatio = static_cast<float>(hp) / maxHp;
    m_fillRender.SetScale({ hpRatio, 1.0f, 1.0f });
    m_fillRender.Update();
}

void PlayerHpUI::Render(RenderContext& rc)
{
    if (IsVisible() && m_player != nullptr)
    {
        m_backgroundRender.Draw(rc);
        m_fillRender.Draw(rc);
        m_hpText.Draw(rc);
    }
}

#include "stdafx.h"
#include "GameUI.h"
#include "HeatGaugeUI.h"
#include "WeaponUI.h"
#include "PlayerHpUI.h"
#include"WeaponWheelUI.h"

GameUI::~GameUI()
{
    // このクラスで生成したヒートゲージUIを破棄する。
    if (m_heatGaugeUI != nullptr)
    {
        DeleteGO(m_heatGaugeUI);
    }

    if (m_weaponUI != nullptr)
    {
        DeleteGO(m_weaponUI);
    }

    if (m_playerHpUI != nullptr)
    {
        DeleteGO(m_playerHpUI);
    }

    if (m_weaponWheelUI != nullptr)
    {
        DeleteGO(m_weaponWheelUI);
    }
}

bool GameUI::Start()
{
    // 生成後の更新・描画はエンジンが呼び出す。
    m_heatGaugeUI = NewGO<HeatGaugeUI>(0, "heatGaugeUI");
    m_weaponUI = NewGO<WeaponUI>(0, "weaponUI");
    m_playerHpUI = NewGO<PlayerHpUI>(0, "playerHpUI");
    m_weaponWheelUI = NewGO<WeaponWheelUI>(0, "weaponWheelUI");

    const Vector3 hpPosition{ -916.0f, 480.0f, 0.0f };
    m_playerHpUI->SetPosition(hpPosition);
    // HPバーの左端に合わせ、その下に武器アイコンを並べる。
    m_weaponUI->SetPosition(hpPosition + Vector3(48.0f, -96.0f, 0.0f));
    return true;
}

//Gameに呼ばせる
void GameUI::SetWeaponWheelVisible(bool isVisible, float stickX, float stickY)
{
    //ホイールを閉じていて、開こうとする瞬間だけ
    if (isVisible && !m_weaponWheelUI->IsVisible())
    {
        m_weaponWheelUI->InitSelectionWeapon();
    }

    m_weaponWheelUI->SetVisible(isVisible);//表示状態を変更

    if (isVisible)//表示をtrueにするなら
    {
        m_weaponWheelUI->UpdateSelection(stickX, stickY);//スティックの倒した量を渡す
    }
}

int GameUI::GetHighlightedWeaponIndex() const
{
    return m_weaponWheelUI->GetHighlightedWeaponIndex();
}
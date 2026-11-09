#include "stdafx.h"
#include "GameUI.h"
#include "HeatGaugeUI.h"
#include "WeaponUI.h"
#include "PlayerHpUI.h"

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
}

bool GameUI::Start()
{
    // 生成後の更新・描画はエンジンが呼び出す。
    m_heatGaugeUI = NewGO<HeatGaugeUI>(0, "heatGaugeUI");
    m_weaponUI = NewGO<WeaponUI>(0, "weaponUI");
    // 画面中央を原点としたUI座標で、左下に配置する。
    m_weaponUI->SetPosition({ -900.0f, -400.0f, 0.0f });

    // 左端・上端からそれぞれ60の余白を取ってHPを配置する。
    m_playerHpUI = NewGO<PlayerHpUI>(0, "playerHpUI");
    m_playerHpUI->SetPosition({ -916.0f, 480.0f, 0.0f });
    return true;
}

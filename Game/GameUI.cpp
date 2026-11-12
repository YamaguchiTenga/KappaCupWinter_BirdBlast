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

    // 左端・上端からそれぞれ60の余白を取ってHPを配置する。
    m_playerHpUI = NewGO<PlayerHpUI>(0, "playerHpUI");
    const Vector3 hpPosition{ -916.0f, 480.0f, 0.0f };
    m_playerHpUI->SetPosition(hpPosition);
    // HPバーの左端に合わせ、その下に武器アイコンを並べる。
    m_weaponUI->SetPosition(hpPosition + Vector3(48.0f, -96.0f, 0.0f));
    return true;
}

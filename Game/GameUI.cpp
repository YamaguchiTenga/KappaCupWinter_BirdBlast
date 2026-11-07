#include "stdafx.h"
#include "GameUI.h"
#include "HeatGaugeUI.h"

GameUI::~GameUI()
{
    // このクラスで生成したヒートゲージUIを破棄する。
    if (m_heatGaugeUI != nullptr)
    {
        DeleteGO(m_heatGaugeUI);
    }
}

bool GameUI::Start()
{
    // 生成後の更新・描画はエンジンが呼び出す。
    m_heatGaugeUI = NewGO<HeatGaugeUI>(0, "heatGaugeUI");
    return true;
}

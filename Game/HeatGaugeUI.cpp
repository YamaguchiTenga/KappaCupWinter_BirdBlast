#include "stdafx.h"
#include "HeatGaugeUI.h"
#include "Player.h"

bool HeatGaugeUI::Start()
{
    m_player = FindGO<Player>("player");

    m_backgroundRender.Init("Assets/UI/HeatGaugeUI/Background.DDS", 300.0f, 100.0f);//表示する幅と高さを指定する。
    m_backgroundRender.SetPosition(m_position);//UIBaseの基準位置を画像へ設定する
    m_backgroundRender.Update();//表示位置・回転・大きさを描画用に反映する。

    //背景と同じ大きさ・位置で、満タンのゲージ画像を重ねる。
    SpriteInitData initData{};//データを入れるためのinitDataというインスタンスを作る。{}中身が空なのではSpriteInitDataに既に入っている初期値をつかうということ
    initData.m_ddsFilePath[0] = "Assets/UI/HeatGaugeUI/Gauge_Fill.DDS";
    initData.m_width = 300;
    initData.m_height = 100;
    initData.m_alphaBlendMode = AlphaBlendMode_Trans;//上に重ねる画像の透明度を見て、下の背景をどれだけ見せるかを決める
    initData.m_fxFilePath = "Assets/shader/heatGauge.fx";
    initData.m_expandConstantBuffer = &m_fillRatio;
    initData.m_expandConstantBufferSize = sizeof(m_fillRatio);
    m_gaugeFillRender.Init(initData);//オーバーロードによりデータを入れていく方が使われる
    m_gaugeFillRender.SetPosition(m_position);
    m_gaugeFillRender.Update();
    return true;
}

void HeatGaugeUI::Update()
{
    if (m_player == nullptr)
    {
        return;
    }

    //現在のヒート量を表示割合に反映する。
    m_fillRatio = m_player->GetAbilityController().GetGaugeRatio();
}

void HeatGaugeUI::Render(RenderContext& rc)
{
    // 非表示に設定されている場合は描画しない。
    if (!IsVisible())
    {
        return;
    }

    m_backgroundRender.Draw(rc);
    m_gaugeFillRender.Draw(rc);//背景の上にゲージ部分を描画する
}

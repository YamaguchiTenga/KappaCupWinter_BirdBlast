#pragma once
#include"UIBase.h"

class Player;

class HeatGaugeUI : public UIBase
{
public:
	bool Start() override;

	void Update() override;
	void Render(RenderContext& rc)override;
private:
	Player* m_player = nullptr;
	float m_fillRatio = 0.0f;//ゲージの表示割合。0で空、1でマックス
private:
	SpriteRender m_backgroundRender;//ゲージの背景画像を表示する
	SpriteRender m_gaugeFillRender;//色付きのゲージ部分を表示する
};


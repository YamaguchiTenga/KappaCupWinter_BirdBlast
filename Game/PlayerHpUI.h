#pragma once
#include "UIBase.h"

class Player;

class PlayerHpUI : public UIBase
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

private:
    const Player* m_player = nullptr;
    SpriteRender m_backgroundRender; // HPバーの背景。
    SpriteRender m_fillRender;       // 残りHPを示す緑のバー。
    FontRender m_hpText;
};

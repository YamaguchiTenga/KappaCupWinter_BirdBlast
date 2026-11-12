#pragma once

class Title : public IGameObject
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

private:
    FontRender m_titleText;
    FontRender m_startText;
};

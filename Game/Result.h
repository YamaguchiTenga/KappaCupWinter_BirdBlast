#pragma once

class Result : public IGameObject
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

private:
    FontRender m_resultText;
    FontRender m_returnText;
};

#pragma once

class Result : public IGameObject
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

    void SetGameOver() { m_isGameOver = true; }

private:
    bool m_isGameOver = false;
    FontRender m_resultText;
    FontRender m_returnText;
};

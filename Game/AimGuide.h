#pragma once
#include "Actor.h"

class AimGuide : public Actor
{
public:
    bool Start() override;
    void Render(RenderContext& rc) override;

    // ガイドを表示する位置を受け取る。
    void SetPosition(const Vector3& position)
    {
        m_transform.m_position = position;
    }

    // 表示・非表示を切り替える。
    void SetVisible(bool isVisible)
    {
        m_isVisible = isVisible;
    }

private:
    bool m_isVisible = false;
};
#include "stdafx.h"
#include "WindVisual.h"

// AI実装：体の周りの風の帯と、後方へ伸びる尾を描く。
bool WindVisual::Start()
{
    ModelInitData visual;
    visual.m_tkmFilePath = "Assets/modelData/Weapons/Wind/WindBands.tkm";
    visual.m_fxFilePath = "Assets/shader/windBands.fx";
    visual.m_modelUpAxis = enModelUpAxisY;
    visual.m_expandConstantBuffer = &m_visualParameters;
    visual.m_expandConstantBufferSize = sizeof(m_visualParameters);
    // 明るさを加算せず、薄い水色を半透明で重ねる。
    visual.m_alphaBlendMode = AlphaBlendMode_Trans;
    visual.m_isDepthWrite = false;
    visual.m_cullMode = D3D12_CULL_MODE_NONE;
    m_modelRender.InitForwardRendering(visual);
    m_modelRender.SetShadowCasterFlag(false);
    return true;
}

void WindVisual::Show(const Vector3& position, const Vector3& direction)
{
    if (!m_isVisible)
    {
        m_visualParameters.x = 0.0f;
    }
    m_isVisible = true;
    m_position = position;
    if (direction.x * direction.x + direction.z * direction.z > 0.000001f)
    {
        m_rotation.SetRotationYFromDirectionXZ(direction);
    }
}

void WindVisual::Hide()
{
    m_isVisible = false;
    m_visualParameters = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void WindVisual::Update()
{
    if (!m_isVisible)
    {
        return;
    }
    m_visualParameters.x += g_gameTime->GetFrameDeltaTime();
    // 発動直後に急に濃くならないよう、0.12秒かけて帯を表示する。
    m_visualParameters.y = m_visualParameters.x / 0.12f;
    if (m_visualParameters.y > 1.0f)
    {
        m_visualParameters.y = 1.0f;
    }
    m_modelRender.SetTRS(m_position, m_rotation, Vector3::One);
    m_modelRender.Update();
}

void WindVisual::Render(RenderContext& rc)
{
    if (m_isVisible)
    {
        m_modelRender.Draw(rc);
    }
}

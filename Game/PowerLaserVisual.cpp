#include "stdafx.h"
#include "PowerLaserVisual.h"

// AI実装：白い芯・水色の光・流れるリングのモデルとシェーダーを初期化する。
bool PowerLaserVisual::Start()
{
    ModelInitData visual;
    visual.m_tkmFilePath = "Assets/modelData/Bullets/PowerLaser_Energy.tkm";
    visual.m_fxFilePath = "Assets/shader/powerLaserEnergy.fx";
    visual.m_modelUpAxis = enModelUpAxisY;
    visual.m_expandConstantBuffer = &m_visualParameters;
    visual.m_expandConstantBufferSize = sizeof(m_visualParameters);
    visual.m_alphaBlendMode = AlphaBlendMode_Add;
    visual.m_isDepthWrite = false;
    visual.m_cullMode = D3D12_CULL_MODE_NONE;
    m_modelRender.InitForwardRendering(visual);
    m_modelRender.SetShadowCasterFlag(false);
    return true;
}

void PowerLaserVisual::Show(const Vector3& position, const Quaternion& rotation)
{
    m_position = position;
    m_rotation = rotation;
    m_widthRatio = 1.0f;
    m_visualParameters.animation.x = 0.0f;
    m_isVisible = true;
}

void PowerLaserVisual::SetWidthRatio(float ratio)
{
    if (ratio < 0.0f)
    {
        ratio = 0.0f;
    }
    else if (ratio > 1.0f)
    {
        ratio = 1.0f;
    }
    m_widthRatio = ratio;
}

void PowerLaserVisual::Hide()
{
    m_isVisible = false;
}

void PowerLaserVisual::Update()
{
    if (!m_isVisible)
    {
        return;
    }
    m_visualParameters.animation.x += g_gameTime->GetFrameDeltaTime();
    const Vector3& eye = g_camera3D->GetPosition();
    m_visualParameters.eyePosition = Vector4(eye.x, eye.y, eye.z, 1.0f);
    const float width = BeamWidthScale * m_widthRatio;
    m_modelRender.SetTRS(m_position, m_rotation, Vector3(width, width, 1.0f));
    m_modelRender.Update();
}

void PowerLaserVisual::Render(RenderContext& rc)
{
    if (m_isVisible)
    {
        m_modelRender.Draw(rc);
    }
}

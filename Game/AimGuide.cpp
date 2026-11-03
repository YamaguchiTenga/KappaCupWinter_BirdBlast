#include "stdafx.h"
#include "AimGuide.h"

bool AimGuide::Start()
{
    // まずは既存の球モデルで、予測位置を確認する。
    m_modelRender.Init("Assets/modelData/Bullets/PlayerSphere.tkm");

    return true;
}

void AimGuide::Render(RenderContext& rc)
{
    if (!m_isVisible)
    {
        return;
    }

    Actor::Render(rc);
}
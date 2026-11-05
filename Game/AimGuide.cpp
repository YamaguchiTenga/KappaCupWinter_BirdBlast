#include "stdafx.h"
#include "AimGuide.h"

bool AimGuide::Start()
{
    //まずは既存モデルで予測位置を確認
    m_modelRender.Init("Assets/modelData/Bullets/PlayerSphere.tkm");

    for (int i = 0; i < m_TrajectoryPointCount; i++)
    {
        m_trajectoryModels[i].Init("Assets/modelData/Bullets/PlayerSphere.tkm");
    }
    return true;
}

void AimGuide::Render(RenderContext& rc)
{
    if (!m_isVisible)
    {
        return;
    }

    Actor::Render(rc);

    for (int i = 0; i < m_visibleTrajectoryCount;i++)
    {
        m_trajectoryModels[i].Draw(rc);
    }
}
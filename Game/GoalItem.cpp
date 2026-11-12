#include "stdafx.h"
#include "GoalItem.h"
#include "Player.h"

bool GoalItem::Start()
{
    m_player = FindGO<Player>("player");
    m_transform.m_position.Set(0.0f, 50.0f, 800.0f);
    m_transform.m_scale.Set(2.0f, 2.0f, 2.0f);
    m_modelRender.Init("Assets/modelData/Bullets/PlayerSphere.tkm");
    Actor::Update();
    return true;
}

void GoalItem::Update()
{
    if (m_isCollected || m_player == nullptr)
    {
        return;
    }

    Vector3 playerCenter = m_player->GetPosition();
    playerCenter.y += 50.0f;
    const float pickupDistance = 60.0f;
    m_isCollected = (playerCenter - m_transform.m_position).Length() <= pickupDistance;
}

void GoalItem::Render(RenderContext& rc)
{
    if (!m_isCollected)
    {
        Actor::Render(rc);
    }
}

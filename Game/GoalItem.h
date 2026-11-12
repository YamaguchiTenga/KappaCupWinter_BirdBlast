#pragma once
#include "Actor.h"

class Player;

class GoalItem : public Actor
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

    bool IsCollected() const { return m_isCollected; }

private:
    const Player* m_player = nullptr;
    bool m_isCollected = false;
};

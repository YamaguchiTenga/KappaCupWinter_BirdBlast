#include "stdafx.h"
#include "Title.h"
#include "Game.h"

bool Title::Start()
{
    m_titleText.SetText(L"BIRD BLAST");
    m_titleText.SetPivot(0.5f, 0.5f);
    m_titleText.SetPosition(-400.0f, 100.0f, 0.0f);
    m_titleText.SetScale(2.5f);

    m_startText.SetText(L"Press A to Start");
    m_startText.SetPivot(0.5f, 0.5f);
    m_startText.SetPosition(-300.0f, -60.0f, 0.0f);
    return true;
}

void Title::Update()
{
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        NewGO<Game>(0, "game");
        DeleteGO(this);
    }
}

void Title::Render(RenderContext& rc)
{
    m_titleText.Draw(rc);
    m_startText.Draw(rc);
}

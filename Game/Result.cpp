#include "stdafx.h"
#include "Result.h"
#include "Title.h"

bool Result::Start()
{
    m_resultText.SetText(L"STAGE CLEAR");
    m_resultText.SetPivot(0.5f, 0.5f);
    m_resultText.SetPosition(-400.0f, 100.0f, 0.0f);
    m_resultText.SetScale(2.5f);

    m_returnText.SetText(L"Press A to Title");
    m_returnText.SetPivot(0.5f, 0.5f);
    m_returnText.SetPosition(-300.0f, -60.0f, 0.0f);
    return true;
}

void Result::Update()
{
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        NewGO<Title>(0, "title");
        DeleteGO(this);
    }
}

void Result::Render(RenderContext& rc)
{
    m_resultText.Draw(rc);
    m_returnText.Draw(rc);
}

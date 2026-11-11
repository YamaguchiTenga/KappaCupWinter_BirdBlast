#pragma once
#include "Actor.h"
#include<vector>

class AimGuide : public Actor
{
public:
    bool Start() override;
    void Render(RenderContext& rc) override;

public:
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

    void SetTrajectoryPoints(const std::vector<Vector3>& trajectoryPoints)
    {
        m_drawLine = false;
        m_trajectoryPoints = trajectoryPoints;

        int pointCount = static_cast<int>(m_trajectoryPoints.size());//要素数

        m_visibleTrajectoryCount = pointCount;//実際に画面に映す数。要素数を代入し、次のifで数を調整する
        if(m_visibleTrajectoryCount > m_TrajectoryPointCount)//最大数を超えたら制限をかける
        {
            m_visibleTrajectoryCount = m_TrajectoryPointCount;
        }

        for (int i = 0;i < m_visibleTrajectoryCount; i++)
        {
            int pointindex = 0;
            if (m_visibleTrajectoryCount > 1)//表示数が1個だった時の0の除算を防ぐ
            {
                pointindex = i * (pointCount - 1) / (m_visibleTrajectoryCount - 1);//現在の要素の番号 × (軌道の点の数 − 1) ÷ (表示個数 − 1)
            }
            m_trajectoryModels[i].SetPosition(m_trajectoryPoints[pointindex]);
            m_trajectoryModels[i].Update();
        }
    }

    void SetLine(const Vector3& start, const Vector3& end)
    {
        m_drawLine = true;
        m_lineStart = start;
        m_lineEnd = end;

        Vector3 lineCenter = (start + end) * 0.5f;//始点と終点の中央

        Vector3 lineOffset = end - start;
        float lineLength = lineOffset.Length();

        Quaternion lineRotation;
        lineRotation.SetRotationYFromDirectionXZ(lineOffset);

        constexpr float lineWidth = 2.0f;
        m_lineModelRender.SetTRS(lineCenter, lineRotation, Vector3(lineWidth, lineWidth, lineLength));
        m_lineModelRender.Update();
    }

private:
    bool m_isVisible = false;
    bool m_drawLine = false;

    int m_visibleTrajectoryCount = 0;//表示する数
    static const int m_TrajectoryPointCount = 10;//配列の個数にはコンパイル時に確定できる整数が必要なのでstatic
    ModelRender m_trajectoryModels[m_TrajectoryPointCount];
    std::vector<Vector3> m_trajectoryPoints;

    Vector3 m_lineStart = Vector3::Zero;
    Vector3 m_lineEnd = Vector3::Zero;

    ModelRender m_lineModelRender;
};
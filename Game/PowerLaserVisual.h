#pragma once

// AI実装：パワーレーザーの見た目専用。照射時間・攻撃判定・ダメージは扱わない。
class PowerLaserVisual : public IGameObject
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

    void Show(const Vector3& position, const Quaternion& rotation);
    void SetWidthRatio(float ratio);
    void Hide();

private:
    // モデルが参照し続けるため、シェーダーへ渡す値をメンバで保持する。
    struct BeamVisualParameters
    {
        Vector4 animation{ 0.0f, 0.0f, 0.0f, 0.0f };
        Vector4 eyePosition{ 0.0f, 0.0f, 0.0f, 1.0f };
    };
    BeamVisualParameters m_visualParameters;
    ModelRender m_modelRender;
    Vector3 m_position = Vector3::Zero;
    Quaternion m_rotation = Quaternion::Identity;
    static constexpr float BeamWidthScale = 3.0f;
    float m_widthRatio = 1.0f;
    bool m_isVisible = false;
};

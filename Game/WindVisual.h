#pragma once

// AI実装：風武器の見た目専用。攻撃判定・移動・ダメージは扱わない。
class WindVisual : public IGameObject
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

    void Show(const Vector3& position, const Vector3& direction);
    void Hide();

private:
    ModelRender m_modelRender;
    Vector3 m_position = Vector3::Zero;
    Quaternion m_rotation = Quaternion::Identity;
    // xは経過秒、yは表示の濃さ。シェーダーから参照するためメンバにする。
    Vector4 m_visualParameters{ 0.0f, 0.0f, 0.0f, 0.0f };
    bool m_isVisible = false;
};

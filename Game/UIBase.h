#pragma once

//画面に表示するUIの共通データを持つ基底クラス。
class UIBase : public IGameObject
{
public:
    //UI全体を配置する基準位置を設定する。
    void SetPosition(const Vector3& position)
    {
        m_position = position;
    }

    //表示状態を保存する。子クラスのRenderで確認して描画を切り替える。
    //非表示でもUpdateは続ける。
    void SetVisible(bool isVisible)
    {
        m_isVisible = isVisible;
    }
public:
    const Vector3& GetPosition() const
    {
        return m_position;
    }

    bool IsVisible() const;

protected:
    //枠・ゲージ・文字は、この位置を基準に子クラスで配置する。
    Vector3 m_position = Vector3::Zero;
    bool m_isVisible = true;
};

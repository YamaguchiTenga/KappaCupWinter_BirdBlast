#pragma once

class HeatGaugeUI;
class WeaponUI;
class PlayerHpUI;
class WeaponWheelUI;

//ゲーム中のUIをまとめて管理するクラス。
class GameUI : public IGameObject
{
public:
    ~GameUI() override;
    bool Start() override;

    void SetWeaponWheelVisible(bool isVisible, float stickX, float stickY);

    int GetHighlightedWeaponIndex() const;//ホイールの方から選択中の武器を受け取ってGameに渡す
private:
    //このクラスで生成・破棄するヒートゲージUI。
    HeatGaugeUI* m_heatGaugeUI = nullptr;
    WeaponUI* m_weaponUI = nullptr;
    PlayerHpUI* m_playerHpUI = nullptr;
    WeaponWheelUI* m_weaponWheelUI = nullptr;
};

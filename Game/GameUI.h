#pragma once

class HeatGaugeUI;

//ゲーム中のUIをまとめて管理するクラス。
class GameUI : public IGameObject
{
public:
    ~GameUI() override;
    bool Start() override;

private:
    //このクラスで生成・破棄するヒートゲージUI。
    HeatGaugeUI* m_heatGaugeUI = nullptr;
};

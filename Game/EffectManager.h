#pragma once

class EffectManager
{
public:
    // エフェクトの登録と再生で共通して使う種類。
    enum class Effect
    {
        LaserHit = 0,
        BombExplosion = 1
    };

    void Initialize();
    static void PlayEffect(Effect effect, const Vector3& position, Vector3 scale);
};

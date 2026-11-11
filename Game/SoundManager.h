#pragma once

class SoundManager
{
public:
    //音声バンクへの登録と再生で共通して使う効果音の種類。
    enum class Effect
    {
        Laser = 0,
        PowerLaser = 1,
        BombReady = 2,
        BombThrow = 3,
        BombExplosion = 4
    };

    void Initialize();
    static void PlaySE(Effect effect);//staticなので他のクラスでincludeするだけで呼べる
};

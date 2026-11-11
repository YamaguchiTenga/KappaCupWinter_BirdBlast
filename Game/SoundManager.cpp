#include "stdafx.h"
#include "SoundManager.h"
#include "sound/SoundEngine.h"

void SoundManager::Initialize()
{
    g_soundEngine->ResistWaveFileBank(static_cast<int>(Effect::Laser), "Assets/SE/Laser.wav");
    g_soundEngine->ResistWaveFileBank(static_cast<int>(Effect::PowerLaser), "Assets/SE/PowerLaser.wav");
}

void SoundManager::PlaySE(Effect effect)
{
    const int soundId = static_cast<int>(effect);//enum classはintにならないので変換
    if (!g_soundEngine->GetWaveFileBank().FindWaveFile(soundId))
    {
        return;
    }

    //発射ごとに再生する。ループしない音は再生終了後にエンジンが破棄する。
    auto* sound = NewGO<SoundSource>(0);
    sound->Init(soundId);
    sound->Play(false);
}

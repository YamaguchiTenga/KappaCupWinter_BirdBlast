#include "stdafx.h"
#include "SoundManager.h"
#include "sound/SoundEngine.h"

void SoundManager::Initialize()
{
    g_soundEngine->ResistWaveFileBank(static_cast<int>(Effect::Laser), "Assets/SE/Laser.wav");
    g_soundEngine->ResistWaveFileBank(static_cast<int>(Effect::PowerLaser), "Assets/SE/PowerLaser.wav");
    g_soundEngine->ResistWaveFileBank(static_cast<int>(Effect::BombReady), "Assets/SE/BombReady.wav");
    g_soundEngine->ResistWaveFileBank(static_cast<int>(Effect::BombThrow), "Assets/SE/BombThrow.wav");
    g_soundEngine->ResistWaveFileBank(static_cast<int>(Effect::BombExplosion), "Assets/SE/BombExplosion.wav");
    g_soundEngine->ResistWaveFileBank(static_cast<int>(Effect::AttackHit), "Assets/SE/AttackHit.wav");
    g_soundEngine->ResistWaveFileBank(static_cast<int>(Effect::EnemyDeath), "Assets/SE/EnemyDeath.wav");
    g_soundEngine->ResistWaveFileBank(static_cast<int>(Effect::WeaponSelected), "Assets/SE/WeaponSelected.wav");
    g_soundEngine->ResistWaveFileBank(static_cast<int>(Effect::WeaponDecided), "Assets/SE/WeaponDecided.wav");
    g_soundEngine->ResistWaveFileBank(StageBgmId, "Assets/BGM/BGM.wav");
}

void SoundManager::PlayStageBGM()
{
    if (m_bgm != nullptr || !g_soundEngine->GetWaveFileBank().FindWaveFile(StageBgmId))
    {
        return;
    }

    m_bgm = NewGO<SoundSource>(0);
    m_bgm->Init(StageBgmId);
    m_bgm->SetVolume(0.35f);
    m_bgm->Play(true);
}

void SoundManager::StopBGM()
{
    if (m_bgm == nullptr)
    {
        return;
    }

    m_bgm->Stop();
    DeleteGO(m_bgm);
    m_bgm = nullptr;
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

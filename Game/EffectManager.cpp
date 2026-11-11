#include "stdafx.h"
#include "EffectManager.h"

void EffectManager::Initialize()
{
    //パスの直前のuはUTF-16文字列に変えるため。この関数は受け取る文字列が他と異なるらしい
    EffectEngine::GetInstance()->ResistEffect(static_cast<int>(Effect::LaserHit),u"Assets/Effect/ToonHit/ToonHit.efkefc");
    EffectEngine::GetInstance()->ResistEffect(static_cast<int>(Effect::BombExplosion),u"Assets/Effect/BombExplosion/BombExplosion.efkefc");
    EffectEngine::GetInstance()->ResistEffect(static_cast<int>(Effect::EnemyDeathSmoke),u"Assets/Effect/EnemyDeath/09_02_Sample/EnemyDeathSmoke.efkefc");
}

void EffectManager::PlayEffect(Effect effect, const Vector3& position,Vector3 scale)
{
    const int effectId = static_cast<int>(effect);

    auto* emitter = NewGO<EffectEmitter>(0);
    emitter->Init(effectId);
    emitter->SetPosition(position);
    emitter->SetScale(scale);
    emitter->Play();
}
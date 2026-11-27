#include "stdafx.h"
#include "WindWeapon.h"
#include "Enemy.h"
#include "EnemyManager.h"

WindWeapon::~WindWeapon()
{
    if (m_collisionObject != nullptr)
    {
        DeleteGO(m_collisionObject);
    }
}

//親のWeaponはStart()が自動で呼ばれないため、生成する側(WeaponController)から初期化する。
void WindWeapon::Initialize(EnemyManager* enemyManager)
{
    m_enemyManager = enemyManager;

    if (m_collisionObject == nullptr)
    {
        m_collisionObject = NewGO<CollisionObject>(0);
        m_collisionObject->CreateSphere(Vector3::Zero, Quaternion::Identity, m_collisionRadius);
        m_collisionObject->SetIsEnableAutoDelete(false);
        m_collisionObject->SetIsEnable(false);
    }
}
WeaponAttackResult WindWeapon::HandleAttack(const WeaponAttackContext& context)
{
    if (!context.isHeld || m_collisionObject == nullptr || m_enemyManager == nullptr)
    {
        AttackCansel();
        return WeaponAttackResult::None;
    }
    else
    {
        m_isActive = true;
    }

    //風を発動する処理を追加していく。
 
    //Power中だけ引き寄せる
    if (context.isPower)
    {
        PullEnemies(context);
    }
    m_collisionObject->SetPosition(context.position);
    m_collisionObject->SetIsEnable(true);
    CheckHit(g_gameTime->GetFrameDeltaTime(), context);
    
    //風はその場で攻撃処理をして、発射結果をプレイヤーへ知らせる必要がない
    //のでNoneを返す
    return WeaponAttackResult::None;
}

void WindWeapon::AttackCansel()
{
    m_isActive = false;
    m_hitRecords.clear();//命中した記録を空にして、次の発動に待ち時間を持ち越さなくする
    if (m_collisionObject != nullptr)
    {
        m_collisionObject->SetIsEnable(false);
    }
}

void WindWeapon::OnDeselected()
{
    AttackCansel();
}

void WindWeapon::CheckHit(float deltaTime, const WeaponAttackContext& context)
{
    float damageInterval = 0.8f; //継続ダメージの間隔（秒）

    if (context.isPower)
    {
        damageInterval = 0.2f;
    }

    int damage = 1;

    // 記録済みの敵の待ち時間を減らす。
    for (HitRecord& record : m_hitRecords)
    {
        record.cooldown -= deltaTime;
    }

    for (Enemy* enemy : m_enemyManager->GetEnemies())
    {
        if (!enemy->IsStart() || !enemy->IsActive())
        {
            continue;
        }

        //当たったら
        if (!m_collisionObject->IsHit(enemy->GetCharacterController()))
        {
            continue;
        }

        //今調べている敵の命中記録を指すポインター。
        HitRecord* hitRecord = nullptr;


        // 記録一覧から、今の敵に対応するものを探す。
        for (HitRecord& record : m_hitRecords)
        {
            if (record.enemy == enemy)
            {
                //コピーではなく、一覧内にある記録のアドレスを覚える。
                //このポインターを通して、元の記録のcooldownを変更できる。
                hitRecord = &record;
                break;// 見つかったので検索を終える。
            }
        }

        const Vector3 knockbackDirection = enemy->GetPosition() - context.position;

        if (hitRecord == nullptr) //今の敵の命中記録がまだない場合
        {
            enemy->TakeDamage(damage, knockbackDirection);

            //この敵と、次にダメージを与えるまでの待ち時間をセットにする。
            HitRecord record;
            record.enemy = enemy;                //今当たった敵を覚える。
            record.cooldown = damageInterval;  //待ち時間を設定する。

            // 作った記録を一覧へ追加する。
            m_hitRecords.push_back(record);
        }
        else if (hitRecord->cooldown <= 0.0f) //記録があり、待ち時間も終わっている場合
        {
            //同じ敵に再びダメージを与える。
            enemy->TakeDamage(damage, knockbackDirection);

            //一覧内の記録の待ち時間を設定し直す。
            hitRecord->cooldown = damageInterval;
        }
        //記録があっても待ち時間が残っていれば、今回はダメージを与えない。
    }
}

void WindWeapon::PullEnemies(const WeaponAttackContext& context)
{
    //引き寄せるスピード
    float pullSpeed = 500.0f;

    for (Enemy* enemy : m_enemyManager->GetEnemies())
    {
        if (!enemy->IsStart() || !enemy->IsActive())
        {
            continue;
        }

        Vector3 toPlayer = context.position - enemy->GetPosition();

        //Lengthでベクトルの長さのみを取る。方向は取らない
        float distance = toPlayer.Length();

        //引き寄せの範囲内
        if (distance <= m_pullRadius)
        {
            //距離がほぼ0じゃないとき
            if (distance > 0.001f)
            {
                //正規化する
                toPlayer.Normalize();

                enemy->SetPullSpeed(toPlayer * pullSpeed);
            }
        }
    }
}
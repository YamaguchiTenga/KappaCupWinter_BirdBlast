#pragma once

struct WeaponAttackContext
{
	bool isPressed = false;
	bool isHeld = false;
	bool isPower = false;
	Vector3 position = Vector3::Zero;
	Vector3 direction = Vector3::AxisZ;
};

// 武器共通の基底クラス。
// IGameObjectは継承せず、WeaponControllerが所有・管理する。
// Start,Updateなどは自動では呼ばれず、WeaponControllerから呼び出す。
class Weapon
{
public:
	Weapon() = default;
	virtual ~Weapon() = default;//処理がないならこの書き方でもいい

	virtual void Update(float deltaTime);

	//構造体の情報をまとめて受け取る。
	//純粋仮想関数（= 0）：ここには処理を書かず、各武器の子クラスで実装する。
	virtual void HandleAttack(const WeaponAttackContext& context) = 0;//Handleは扱うという意味

private:
	float m_fireInterval = 1.0f; //発射間隔（秒）
	float m_fireCooldown = 0.0f; //次に撃てるまでの残り時間（秒）

protected://親と子クラスのみ使える
	//各武器が発射可能かを調べれるようにする
	bool CanFire() const
	{
		return m_fireCooldown <= 0.0f;
	}

	void StartCooldown()
	{
		m_fireCooldown = m_fireInterval;//インターバルを入れる
	}

	void SetFireInterval(float fireInterval)//武器ごとにインターバルを設定
	{
		m_fireInterval = fireInterval;
	}
};


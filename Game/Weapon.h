#pragma once

//各武器に共通する基底クラス。
//発射待ち時間を管理し、具体的な攻撃方法は派生クラスで実装する。


struct WeaponAttackContext//Player::Attack()でこの変数を作る
{
	bool isPressed = false;
	bool isHeld = false;
	bool isPower = false;
	Vector3 position = Vector3::Zero;
	Vector3 direction = Vector3::AxisZ;

	//照準調整用の入力。Xは左右、Yは上下。
	Vector3 aimInput = Vector3::Zero;
};

class AimGuide;

//武器共通の基底クラス。
//IGameObjectは継承せず、WeaponControllerが所有・管理する。
//Start,Updateなどは自動では呼ばれず、WeaponControllerから呼び出す。
class Weapon
{
public:
	Weapon() = default;
	virtual ~Weapon();
	virtual void Update(float deltaTime);

	//構造体の情報をまとめて受け取る。
	//純粋仮想関数（= 0）：ここには処理を書かず、各武器の子クラスで実装する。
	virtual void HandleAttack(const WeaponAttackContext& context) = 0;//Handleは扱うという意味

	//パワーレーザー用の移動制限
	//通常は移動を制限しない。必要な武器だけ上書きする。
	virtual bool IsMovementLocked() const
	{
		return false;
	}

	//移動制限
	virtual bool IsMoveInputLocked() const
	{
		return false;
	}

	// この武器の選択を外すときに呼ぶ。
	//後始末が必要な武器だけ上書きする。
	virtual void OnDeselected()
	{
	}
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
protected:
	bool m_wasAttackHeld = false;	//前フレームに射撃ボタンが押されていたか。
	void InitializeAimGuide();
	AimGuide* m_aimGuide = nullptr;
	//発射位置から前方直線へ、照準表示に使う点を並べる。
	static void BuildStraightTrajectory(//親クラスのメンバ変数は使わないのでstatic
		const Vector3& position,
		const Vector3& direction,
		float length,
		std::vector<Vector3>& trajectoryPoints);
protected:
	void SetFireInterval(float fireInterval)//武器ごとにインターバルを設定
	{
		m_fireInterval = fireInterval;
	}
};


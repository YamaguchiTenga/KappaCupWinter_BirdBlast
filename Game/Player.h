#pragma once
#include"Actor.h"
#include "AbilityController.h"
#include "WeaponController.h"
class BulletManager;
class Player : public Actor
{
public:
	Player();
	~Player();

	bool Start()override;

	void Update()override;

	void Move();

	void Jump();

	void Rotation();

	void Render(RenderContext& rc)override;

	void Attack();


	const Vector3& GetPosition() const
	{
		return m_transform.m_position;
	}

	bool IsOnGround()
	{
		return m_characterController.IsOnGround();
	}

	CharacterController& GetCharacterController()
	{
		return m_characterController;
	}

	const AbilityController& GetAbilityController() const
	{
		return m_abilityController;
	}

	//WeaponControllerはプレイヤーのメンバでprivateになっているので、プレイヤーを経由して情報を得る
	int GetSelectedWeaponIndex() const
	{
		return m_weaponController.GetSelectedWeaponIndex();
	}
	//同様にプレイヤーを経由して選択武器を変える
	void SelectWeapon(int weaponIndex)
	{
		m_weaponController.SelectWeapon(weaponIndex);
	}

	void TakeDamage(int damage, const Vector3& knockbackDirection = Vector3::Zero);

	int GetHp() const
	{
		return m_hp;
	}

	int GetMaxHp() const
	{
		return m_maxHp;
	}

	void UpdateAbility();
private:
	void UpdateAnimation();
	void UpdateWeaponSelection();
private:
	static constexpr int ShootAnimation = 2;
	static constexpr int BombHoldAnimation = 3;
	AnimationClip m_animationClips[4];
	ModelRender m_heldBombRender;
	ModelRender m_heldPowerBombRender;
	bool m_isHoldingBomb = false;
	bool m_isHoldingPowerBomb = false;
	int m_currentAnimation = 0;

	Vector3 m_moveSpeed = Vector3::Zero;
	Vector3 m_inputMoveSpeed = Vector3::Zero;
	Vector3 m_externalMoveSpeed = Vector3::Zero;
	float KnockbackSpeed = 800.0f;
	float KnockbackDeceleration = 4000.0f;
	CharacterController m_characterController;
	float m_jumpPower = 500.0f;
	float m_verticalSpeed = 0.0f;//verticalは縦という意味
	float m_gravity = 1500.0f; 

	float m_aimTurnSpeed = 200.0f;//構え中に左右入力を最大まで倒したとき、1秒で回転する角度。

	BulletManager* m_bulletManager = nullptr;

	int m_maxHp = 100;
	int m_hp = m_maxHp;
	float m_invincibleTimeRemaining = 0.0f;

	AbilityController m_abilityController;

	WeaponController m_weaponController;

};


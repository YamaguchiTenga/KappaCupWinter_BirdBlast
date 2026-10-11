#pragma once

class Bullet;

class BulletManager : public IGameObject
{
public:
	BulletManager();
	~BulletManager();

	bool Start();

	void Update();

	void RequestFire(const Vector3& position, const Vector3& moveSpeed);
private:
	static const int BulletCount = 10;//配列のサイズに使える定数にしている
	Bullet* m_bullets[BulletCount] = {};
};


#pragma once
class BulletManager : public IGameObject
{
public:
	BulletManager();
	~BulletManager();

	bool Start();

	void Update();
private:
};


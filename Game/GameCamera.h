#pragma once

class Player;
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();

	bool Start();
	void Update();

private:
	Player* m_player;

	//注視点からカメラまでのベクトル
	Vector3 m_toCameraPosition  = Vector3::Zero;
};


#pragma once
#include "camera/CameraCollisionSolver.h"

class Player;
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();

	bool Start();
	void Update();

private:
	// 地形との衝突を調べ、安全なカメラ位置を求めるエンジンの機能。
	CameraCollisionSolver m_cameraCollisionSolver;

	Player* m_player;
	float m_followY = 0.0f;

	//注視点からカメラまでのベクトル
	Vector3 m_toCameraPosition  = Vector3::Zero;
};


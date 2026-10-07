#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class GameCamera;
class Stage;
class Enemy;

class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	Player* m_player;
	GameCamera* m_gameCamera;
	Stage* m_stage;
	Enemy* m_enemy;

	ModelRender m_modelRender;
	Vector3 m_pos;
};


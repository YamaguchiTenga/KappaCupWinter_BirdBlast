#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class GameCamera;

class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	Player* player;
	GameCamera* gameCamera;
	ModelRender m_modelRender;
	Vector3 m_pos;
};


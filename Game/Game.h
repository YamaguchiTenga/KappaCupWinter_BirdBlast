#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class GameCamera;
class Stage;
class Enemy;
class Bullet;
class BulletManager;
class EnemyManager;

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
	Bullet* m_bullet;
	BulletManager* m_bulletManager;
	EnemyManager* m_enemyManager;

	ModelRender m_modelRender;
};


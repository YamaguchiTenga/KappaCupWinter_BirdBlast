#pragma once

#include "Level3DRender/LevelRender.h"
#include "SoundManager.h"
#include "EffectManager.h"

class Player;
class GameCamera;
class Stage;
class Enemy;
class Bullet;
class BulletManager;
class EnemyManager;
class GameUI;
class GoalItem;

class Game : public IGameObject
{
public:
	Game() {}
	~Game();
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
	GameUI* m_gameUI = nullptr;
	GoalItem* m_goalItem = nullptr;
	SoundManager m_soundManager;
	EffectManager m_effectManager;

	ModelRender m_modelRender;
};


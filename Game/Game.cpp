#include "stdafx.h"
#include "Game.h"
#include"Player.h"
#include"GameCamera.h"
#include"Stage.h"
#include"Enemy.h"
#include"Bullet.h"
#include"BulletManager.h"
#include"EnemyManager.h"
bool Game::Start()
{
	m_player = NewGO<Player>(0, "player");
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	m_stage = NewGO<Stage>(0, "stage");
	//m_enemy = NewGO<Enemy>(0, "enemy");
	//m_bullet = NewGO<Bullet>(0, "bullet");
	m_bulletManager = NewGO<BulletManager>(0, "bulletManager");
	m_enemyManager = NewGO<EnemyManager>(0, "enemyManager");
	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{

}
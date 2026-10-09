#include "stdafx.h"
#include "Game.h"
#include"Player.h"
#include"GameCamera.h"
#include"Stage.h"
#include"Enemy.h"
#include"Bullet.h"
bool Game::Start()
{
	m_player = NewGO<Player>(0, "player");
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	m_stage = NewGO<Stage>(0, "stage");
	m_enemy = NewGO<Enemy>(0, "enemy");
	m_bullet = NewGO<Bullet>(0, "bullet");

	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{

}
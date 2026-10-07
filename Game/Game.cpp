#include "stdafx.h"
#include "Game.h"
#include"Player.h"
#include"GameCamera.h"
#include"Stage.h"

bool Game::Start()
{
	m_player = NewGO<Player>(0, "player");
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	m_stage = NewGO<Stage>(0, "stage");
	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{

}
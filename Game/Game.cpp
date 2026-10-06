#include "stdafx.h"
#include "Game.h"
#include"Player.h"
#include"GameCamera.h"

bool Game::Start()
{
	player = NewGO<Player>(0, "player");
	gameCamera = NewGO<GameCamera>(0, "gameCamera");
	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{

}
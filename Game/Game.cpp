#include "stdafx.h"
#include "Game.h"
#include"Player.h"
#include"GameCamera.h"
#include"Stage.h"
#include"Enemy.h"
#include"Bullet.h"
#include"BulletManager.h"
#include"EnemyManager.h"
#include "GameUI.h"
#include "GoalItem.h"
#include "Result.h"
#include "gameObject/GameObjectManager.h"
Game::~Game()
{
	m_soundManager.StopBGM();
	DeleteGO(m_gameCamera);
	DeleteGO(m_stage);
	DeleteGO(m_enemyManager);
	DeleteGO(m_bulletManager);
	DeleteGO(m_player);
	if (m_gameUI != nullptr)
	{
		DeleteGO(m_gameUI);
	}
	if (m_goalItem != nullptr)
	{
		DeleteGO(m_goalItem);
	}
}

bool Game::Start()
{
	m_soundManager.Initialize();
	m_soundManager.PlayStageBGM();
	m_effectManager.Initialize();

	m_player = NewGO<Player>(0, "player");
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	m_stage = NewGO<Stage>(0, "stage");
	//m_enemy = NewGO<Enemy>(0, "enemy");
	//m_bullet = NewGO<Bullet>(0, "bullet");
	m_bulletManager = NewGO<BulletManager>(0, "bulletManager");
	m_enemyManager = NewGO<EnemyManager>(0, "enemyManager");
	m_stage->SetBattleDependencies(m_player, m_enemyManager);
	m_gameUI = NewGO<GameUI>(0, "gameUI");
	m_goalItem = NewGO<GoalItem>(0, "goalItem");
	return true;
}

void Game::Update()
{
	UpdateWeaponWheel();
	if (m_wasLtHeld)
	{
		return;
	}

	if (m_player->GetHp() <= 0)
	{
		Result* result = NewGO<Result>(0, "result");
		result->SetGameOver();
		DeleteGO(this);
		return;
	}

	if (m_goalItem->IsCollected())
	{
		NewGO<Result>(0, "result");
		DeleteGO(this);
	}
}

void Game::UpdateWeaponWheel()
{
	const bool isLtHeld = g_pad[0]->IsPress(enButtonLB2);
	GameObjectManager::GetInstance()->SetPaused(isLtHeld ? this : nullptr);

	m_gameUI->SetWeaponWheelVisible(
		isLtHeld,
		g_pad[0]->GetRStickXF(),
		g_pad[0]->GetRStickYF());

	if (m_wasLtHeld && !isLtHeld)
	{
			m_player->SelectWeapon(m_gameUI->GetHighlightedWeaponIndex());
	}

	m_wasLtHeld = isLtHeld;
}
void Game::Render(RenderContext& rc)
{
}

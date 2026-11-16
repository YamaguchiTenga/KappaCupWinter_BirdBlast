#pragma once
#include "BattleArea.h"

class Player;
class Stage : public IGameObject
{
public:

	Stage();
	~Stage();
	bool Start();
	void Update();
	void SetBattleDependencies(const Player* player, EnemyManager* enemyManager);
	void Render(RenderContext& rc);

private:
	ModelRender m_modelRender;
	PhysicsStaticObject m_physicsStaticObject;
	const Player* m_player = nullptr;
	EnemyManager* m_enemyManager = nullptr;
	BattleArea m_battleArea;
	BattleArea m_archerBattleArea;
};


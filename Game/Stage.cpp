#include "stdafx.h"
#include "Stage.h"
#include "EnemyManager.h"
#include "Player.h"

bool Stage::Start()
{
	//sky = NewGO<SkyCube>(0, "sky");
	//sky->SetType(enSkyCubeType_Day);//背景を空にする
	//g_renderingEngine->SetAmbientByIBLTexture(sky->GetTextureFilePath(), 1.0f);//光の度合い

	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();//当たり判定可視化
	m_modelRender.Init("Assets/modelData/Floor/Forest/Forest.tkm", nullptr, 0, enModelUpAxisY);
	m_modelRender.Update();

	ModelRender collisionModel;
	collisionModel.Init("Assets/modelData/Floor/Forest/ForestCollision.tkm", nullptr, 0, enModelUpAxisY);
	collisionModel.Update();
	m_physicsStaticObject.CreateFromModel(collisionModel.GetModel(), collisionModel.GetModel().GetWorldMatrix());
	m_battleArea.Initialize(300.0f, 600.0f, 700.0f,
		{
			{ Vector3(-100.0f, 0.0f, 550.0f), EnemyType::Mushroom },
			{ Vector3(100.0f, 0.0f, 550.0f), EnemyType::Mushroom }
		});
	m_archerBattleArea.Initialize(1100.0f, 1350.0f, 2200.0f,
		{
			{ Vector3(-820.0f, 144.0f, 1470.0f), EnemyType::GoblinArcher },
			{ Vector3(830.0f, 144.0f, 1770.0f), EnemyType::GoblinArcher },
			{ Vector3(-525.0f, 96.0f, 1900.0f), EnemyType::GoblinArcher },
			{ Vector3(-530.0f, 0.0f, 1240.0f), EnemyType::Mushroom },
			{ Vector3(550.0f, 0.0f, 1320.0f), EnemyType::Mushroom },
			{ Vector3(0.0f, 0.0f, 1690.0f), EnemyType::Mushroom },
			{ Vector3(-1080.0f, 0.0f, 1770.0f), EnemyType::Mushroom },
			{ Vector3(650.0f, 0.0f, 2040.0f), EnemyType::Mushroom }
		}, &m_battleArea);
	return true;
}

Stage::Stage()
{

}

Stage::~Stage()
{

}

void Stage::SetBattleDependencies(const Player* player, EnemyManager* enemyManager)
{
	m_player = player;
	m_enemyManager = enemyManager;
}

void Stage::Update()
{
	m_modelRender.Update();

	if (m_player != nullptr && m_enemyManager != nullptr)
	{
		const Vector3& playerPosition = m_player->GetPosition();
		const float deltaTime = g_gameTime->GetFrameDeltaTime();
		m_battleArea.Update(playerPosition, *m_enemyManager, deltaTime);
		m_archerBattleArea.Update(playerPosition, *m_enemyManager, deltaTime);
	}
}

void Stage::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_battleArea.Render(rc);
	m_archerBattleArea.Render(rc);
}
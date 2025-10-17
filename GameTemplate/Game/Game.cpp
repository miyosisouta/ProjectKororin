#include "stdafx.h"
#include "Game.h"
#include "GameManager.h"
#include "SphereCamera.h"
#include "Stage.h"
#include "Actor/Sphere/Player.h"
#include "Actor/Sphere/Sphere.h"
#include "InputSystem.h"
#include "Collision/CollisionManager.h"
#include "Core/LateStageObjectUpdateManager.h"

Game::~Game()
{
	DeleteGO(sphere_);
	DeleteGO(m_player);
	DeleteGO(sphereCamera_);
	DeleteGO(stage_);

	CollisionHitManager::Delete();
	LateStageObjectUpdateManager::Get().UnRegisterSphere();
	LateStageObjectUpdateManager::Delete();
}

bool Game::Start()
{
	gameManager_ = FindGO<GameManager>("gameManager");	//!< ゲームマネージャー
	//m_player = NewGO<Player>(0,"player"); // プレイヤー
	sphere_ = NewGO<Sphere>(0,"sphere"); // 塊
	sphereCamera_ = NewGO<SphereCamera>(0, "sphereCamera"); // 塊のカメラ
	stage_ = NewGO<Stage>(0,"stage"); // ステージ

	auto* inputSystem = NewGO<InputSystem>(0, "inputSystem");
	inputSystem->SetTarget(sphere_);

	CollisionHitManager::Create();
	LateStageObjectUpdateManager::Create();
	LateStageObjectUpdateManager::Get().RegisterSphere(sphere_);

	return true;
}

void Game::Update()
{
	CollisionHitManager::Get().Update();
	LateStageObjectUpdateManager::Get().Update();
}

void Game::Render(RenderContext& rc)
{

}
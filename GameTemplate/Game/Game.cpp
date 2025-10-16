#include "stdafx.h"
#include "Game.h"
#include "GameManager.h"
#include "SphereCamera.h"
#include "Stage.h"
#include "Actor/Player.h"
#include "Actor/Sphere.h"
#include "InputSystem.h"
#include "Collision/CollisionManager.h"

Game::~Game()
{
	DeleteGO(m_sphere);
	DeleteGO(m_player);
	DeleteGO(m_sphereCamera);
	DeleteGO(m_stage);

	CollisionHitManager::Delete();
}

bool Game::Start()
{
	m_gameManager = FindGO<GameManager>("gameManager");	//!< ゲームマネージャー
	//m_player = NewGO<Player>(0,"player"); // プレイヤー
	m_sphere = NewGO<Sphere>(0,"sphere"); // 塊
	m_sphereCamera = NewGO<SphereCamera>(0, "sphereCamera"); // 塊のカメラ
	m_stage = NewGO<Stage>(0,"stage"); // ステージ

	auto* inputSystem = NewGO<InputSystem>(0, "inputSystem");
	inputSystem->SetTarget(m_sphere);

	CollisionHitManager::Create();

	return true;
}

void Game::Update()
{
	CollisionHitManager::Get().Update();
}

void Game::Render(RenderContext& rc)
{

}
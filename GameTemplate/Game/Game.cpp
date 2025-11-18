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
#include "Core/InGameManager.h"
#include "Core/GameUIManager.h"
#include "UI/Canvas.h"


Game::~Game()
{
	DeleteGO(sphere_);
	DeleteGO(m_player);
	DeleteGO(sphereCamera_);
	DeleteGO(stage_);

	CollisionHitManager::Delete();
	LateStageObjectUpdateManager::Get().UnRegisterSphere();
	LateStageObjectUpdateManager::Delete();
	InGameManager::DeleteInstance();
	GameUIManager::DeleteInstance();
}


bool Game::Start()
{
	/* 必要なオブジェクトの作成 */
	sphere_ = NewGO<Sphere>(0,"sphere"); // 塊
	sphereCamera_ = NewGO<SphereCamera>(0, "sphereCamera"); // 塊のカメラ
	stage_ = NewGO<Stage>(0,"stage"); // ステージ
	canvas_ = NewGO<Canvas>(0, "canvas"); // キャンバス
	auto* inputSystem = NewGO<InputSystem>(0, "inputSystem"); // 操作用クラスの作成と操作する物の設定

	// アップデートの処理順番を設定
	// NewGO<クラス名>(数字：実行する順番を設定できる)
	NewGO<InGameUpdateObject>(GameObjectPriority::InGameManager);
	NewGO<InGameLateUpdateObject>(GameObjectPriority::InGameManagerLate);
	NewGO<GameUIUpdate>(GameObjectPriority::UI);


	/* セッター */
	inputSystem->SetTarget(sphere_);
	sphereCamera_->SetTarget(sphere_);


	/* マネージャーのインスタンスを作成 */
	CollisionHitManager::CreateInstance();
	LateStageObjectUpdateManager::CreateInstance();
	LateStageObjectUpdateManager::Get().RegisterSphere(sphere_);
	InGameManager::CreateInstance();
	GameUIManager::CreateInstance();
	

	return true;
}


void Game::Update()
{
	CollisionHitManager::Get().Update();
	LateStageObjectUpdateManager::Get().Update();


	CheckAndHandleGameState(); // クリアチェック
	
}

void Game::Render(RenderContext& rc)
{
}




void Game::CheckAndHandleGameState()
{

}


#include "stdafx.h"
#include "Game.h"
#include "GameManager.h"
#include "SphereCamera.h"
#include "Stage.h"
#include "Actor/Sphere/Player.h"
#include "Actor/Sphere/Sphere.h"
#include "InputSystem.h"
#include "Collision/CollisionManager.h"
#include "Core/Fade.h"
#include "Core/GameTimer.h"
#include "Core/GameUIManager.h"
#include "Core/InGameManager.h"
#include "Core/LateStageObjectUpdateManager.h"
#include "UI/Canvas.h"


namespace
{
	const float GAME_TIMER_LIMIT = 30.0f;
}


Game::~Game()
{
	DeleteGO(sphere_);
	DeleteGO(sphereCamera_);
	DeleteGO(stage_);
	DeleteGO(canvas_);

	CollisionHitManager::Delete();
	LateStageObjectUpdateManager::Get().UnRegisterSphere();
	LateStageObjectUpdateManager::Delete();
	InGameManager::DeleteInstance();
	GameUIManager::DeleteInstance();
	GameTimer::DestroyInstance();
}


bool Game::Start()
{
	/* 必要なオブジェクトの作成 */
	sphere_ = NewGO<Sphere>(0,"sphere"); // 塊
	sphereCamera_ = NewGO<SphereCamera>(0, "sphereCamera"); // 塊のカメラ
	stage_ = NewGO<Stage>(0,"stage"); // ステージ
	canvas_ = NewGO<Canvas>(0, "canvas"); // キャンバス
	NewGO<FadeObject>(0, "fadeObject");
	auto* inputSystem = NewGO<InputSystem>(0, "inputSystem"); // 操作用クラスの作成と操作する物の設定


	/* アップデートの処理順番を設定 */
	// NewGO<クラス名>(数字：実行する順番を設定できる)
	NewGO<InGameUpdateObject>(GameObjectPriority::InGameManager);
	NewGO<InGameLateUpdateObject>(GameObjectPriority::InGameManagerLate);
	NewGO<GameUIUpdate>(GameObjectPriority::UI);


	/* マネージャーのインスタンスを作成 */
	CollisionHitManager::CreateInstance();
	LateStageObjectUpdateManager::CreateInstance();
	LateStageObjectUpdateManager::Get().RegisterSphere(sphere_);
	InGameManager::CreateInstance();
	GameUIManager::CreateInstance();
	GameTimer::CreateInstance();


	/* セッター */
	inputSystem->SetTarget(sphere_); // 操作ターゲットの指定
	sphereCamera_->SetTarget(sphere_); // カメラのターゲットの指定
	GameTimer::Get().SetGameTime(GAME_TIMER_LIMIT); // 時間用クラスにゲームの制限時間を伝える
	GameTimer::Get().Init();
	

	return true;
}


void Game::Update()
{
	CollisionHitManager::Get().Update();
	LateStageObjectUpdateManager::Get().Update();
	GameTimer::Get().Update();


	canvas_->SetTimer(GameTimer::Get().GetRemainingTime()); // 残り時間をUIに伝える
	
	if (GameTimer::Get().GetRemainingTime() == 0.0f) 
	{
		CheckAndHandleGameState(); // クリアチェック
	}
	
	
}


void Game::CheckAndHandleGameState()
{
	GameManager::Get().SetNextScene(GameManager::SceneState::EnResult);
	DeleteGO(this);
}


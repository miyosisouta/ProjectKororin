#include "stdafx.h"
#include "GameScene.h"
#include "InputSystem.h"
#include "SphereCamera.h"
#include "Stage.h"
#include "Actor/Sphere/Player.h"
#include "Actor/Sphere/Sphere.h"
#include "Collision/CollisionManager.h"
#include "Core/Fade.h"
#include "Core/GameTimer.h"
#include "Core/GameUIManager.h"
#include "Core/InGameManager.h"
#include "Core/LateStageObjectUpdateManager.h"
#include "Scene/ResultScene.h"
#include "UI/Canvas.h"
#include "UI/Util.h"


namespace
{
	constexpr const float FADE_OUT_START_TIME = 2.0f;			// フェードアウトが始まるまでの時間
	constexpr const float FLIGHT_START_DELAY = 2.5f;			// オブジェクトが空へ飛ぶまでの時間
	constexpr const float GAME_TIMER_LIMIT = 300.0f;				// ゲーム時間
	constexpr const int METERS_TO_CENTIMETERS = 100;			// メートルとセンチメートルを分ける
	static const Vector3 SPRITE_BUTTON_POS = Vector3(-300.0f, -300.0f, 0.0f); // ボタンの画像の座標
	static const Vector3 FONT_BUTTON_POS = Vector3(-100.0f, -300.0f, 0.0f); // ボタンテキストの画像の座標
	static const Vector3 BLACK_OBJECT_INIT_POS = Vector3(0.0f, 700.0f, 0.0f); // 黒い背景用オブジェクトの最初の位置
	static const Vector3 BLACK_OBJECT_LAST_POS = Vector3(0.0f, 0.0f, 0.0f); // 黒い背景用オブジェクトの最終的な位置

	// フェードの境界時間を定数として定義
	constexpr const float FADE_START_TIME = 20.0f; // フェード開始 (α = 0.0f)
	constexpr const float FADE_MID_TIME = 10.0f; // 中間点 (α = 0.3f)
	constexpr const float FADE_END_TIME = 0.0f;  // フェード終了 (α = 1.0f)

	constexpr const float STAGE_DURATION = 10.0f; // 各ステージの長さ (10秒)
	constexpr const float ALPHA_MAX_FIRST_HALF = 0.3f;  // ステージ1の最大アルファ値
	constexpr const float ALPHA_RANGE_SECOND_HALF = 0.7f;  // ステージ2のアルファ値の増加幅 (1.0f - 0.3f)

	/** タイムを秒に変換 */
	int CalcMinuteToSecond(int clearTime, int& clearTimeMinute, int& clearTimeSecond)
	{
		clearTimeMinute = clearTime / 60; // 分を返す
		clearTimeSecond = clearTime % 60; // 秒を返す
		return 1;
	}
}



namespace _internal
{
	// static変数初期化
	std::array<Result::State, Result::Step::Max> Result::stepList_;


	Result::Result(GameScene* owner)
		: owner_(owner)
	{
	}


	Result::~Result()
	{
	}


	void Result::Start()
	{
		auto& currentState = stepList_[currentStep_];
		currentState.enter(this);
	}


	void Result::Update()
	{
		auto& currentState = stepList_[currentStep_];
		if (nextStep_ != Step::Invalid && nextStep_ != currentStep_)
		{
			currentState.exit(this);
			currentState = stepList_[nextStep_];
			currentStep_ = nextStep_;
			currentState.enter(this);
		}
		currentState.update(this);
	}


	void Result::Render(RenderContext& rc)
	{
		if (blackOutObject_) {
			blackOutObject_->Draw(rc);
		}

		// クリア時の表示
		{
			if (resultGuidanceSizeText_) {
				resultGuidanceSizeText_->Draw(rc);
			}
			if (resultSphereSizeText_) {
				resultSphereSizeText_->Draw(rc);
			}
			if (resultGuidanceGoalTime_) {
				resultGuidanceGoalTime_->Draw(rc);
			}
			if (goalTimeText_) {
				goalTimeText_->Draw(rc);
			}
			if (resultGuidanceAttachCountText_) {
				resultGuidanceAttachCountText_->Draw(rc);
			}
			if (attachableObjectCountText_) {
				attachableObjectCountText_->Draw(rc);
			}
		}
		// クリアできなかったとき
		{
			if (failureText_) {
				failureText_->Draw(rc);
			}
			if (buttonSprite_) {
				buttonSprite_->Draw(rc);
			}
			if (buttonText_) {
				buttonText_->Draw(rc);
			}
		}
	}


	void Result::Initialize()
	{
		auto setting = [&](const Step::Enum step, EnterFunc enter, UpdateFunc update, ExitFunc exit)
			{
				if (stepList_[step].enter == nullptr) {
					stepList_[step].enter = enter;
				}
				if (stepList_[step].update == nullptr) {
					stepList_[step].update = update;
				}
				if (stepList_[step].exit == nullptr) {
					stepList_[step].exit = exit;
				}
			};

		// step1
		setting(Step::Step1, EnterStep1, UpdateStep1, ExitStep1);
		// step2
		setting(Step::Step2, EnterStep2, UpdateStep2, ExitStep2);
		// step3
		setting(Step::Step3, EnterStep3, UpdateStep3, ExitStep3);
		// step4
		setting(Step::Step4, EnterStep4, UpdateStep4, ExitStep4);
		// step5
		setting(Step::Step5, EnterStep5, UpdateStep5, ExitStep5);
	}


	void Result::EnterStep1(Result* result)
	{
		// 今のInputSystemを削除し、タイトル用のInputSystemを作成
		DeleteGO(result->owner_->inputSystem_);
		result->owner_->inputSystem_ = NewGO<TitleInputSyste>(0, "inputSystem");
		result->owner_->inputSystem_->SetTarget(result->owner_->sphere_);

		// クリアしていない場合
		if (!result->owner_->sphere_->CheakGoalSize())
		{
			// 背景用オブジェクトの作成・設定
			result->blackOutObject_ = std::make_unique<ModelRender>();
			result->blackOutObject_->Init("Assets/modelData/stage/result/blackOutObject_second.tkm");
			result->blackOutObject_->SetPosition(BLACK_OBJECT_INIT_POS);
			result->blackOutObject_->SetScale(10.0f);
			result->blackOutObject_->Update();

			// 塊は描画しない
			result->owner_->sphere_->SetIsDraw(false);

			// オブジェクト非表示
			result->owner_->stage_->SetAllVisible(false);
			result->owner_->stage_->SetVisibleAttackObject(false);

			// ディレクションンライトのパラメーター設定
			g_sceneLight->SetDirectionLight(0, Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
		}

		// フェード処理
		Fade::Get().PlayFade(FadeMode::FadeIn, FADE_OUT_START_TIME);
	}
	void Result::UpdateStep1(Result* result)
	{
		// ホワイトイン完了した場合
		if (!Fade::Get().IsPlay())
		{
			result->isMoveBlackOutObject_ = true;
			result->nextStep_ = Step::Step2;
		}
	}
	void Result::ExitStep1(Result* result)
	{
	}


	void Result::EnterStep2(Result* result)
	{
		// リープで使用する時間の設定
		result->calclerpValue_.SetTargetTime(7.0f);
	}
	void Result::UpdateStep2(Result* result)
	{
		// オブジェクトのリープ
		if (result->blackOutObject_) {
			Vector3 deltaPosition = BLACK_OBJECT_LAST_POS - result->blackOutObject_->GetPosition(); // 移動先と今の座標の差
			if (deltaPosition.Length() >= 0.5f)  // まだ移動させたい場合
			{

				const float lerpValue = result->calclerpValue_.CalcUpdate();

				Vector3 currentPos = result->blackOutObject_->GetPosition(); // 今いる座標
				currentPos.Lerp(lerpValue, BLACK_OBJECT_INIT_POS, BLACK_OBJECT_LAST_POS); // 移動先の座標を線形補完
				result->blackOutObject_->SetPosition(currentPos); // 移動先のポジションを設定
				result->blackOutObject_->Update();

				return;
			}
		}
		result->nextStep_ = Step::Step3;
	}
	void Result::ExitStep2(Result* result)
	{

	}


	void Result::EnterStep3(Result* result)
	{
		if (result->owner_->sphere_->CheakGoalSize())
		{
			// クリアのとき

			// 大きさを表示
			{
				result->resultGuidanceSizeText_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->resultGuidanceSizeText_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, 256, L"大きさ");
					});
				result->resultGuidanceSizeText_->SetPSC(Vector3(300.0f, 350.0f, 0.0f), 1.0f, Vector4(180.0f / 255.0f, 241.0f / 255.0f, 249.0f / 255.0f, 1.0f));
			}

			// 塊の大きさの表示
			{
				const int radiusMetersText = result->information_.scale / METERS_TO_CENTIMETERS; // メートルを算出
				const int radiusCentimeters = (int)result->information_.scale % METERS_TO_CENTIMETERS; // センチメートルを算出

				result->resultSphereSizeText_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->resultSphereSizeText_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, 256, L"%02d m %02d cm", radiusMetersText, radiusCentimeters);
					});
				result->resultSphereSizeText_->SetPSC(Vector3(300.0f, 300.0f, 0.0f), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			}


			// 目標達成を表示
			{
				result->resultGuidanceGoalTime_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->resultGuidanceGoalTime_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, 256, L"達成時間");
					});
				result->resultGuidanceGoalTime_->SetPSC(Vector3(300.0f, 200.0f, 0.0f), 1.0f, Vector4(65.0f / 255.0f, 130.0f / 255.0f, 250.0f / 255.0f, 1.0f));
			}

			// 目標サイズ達成時間の表示
			{
				CalcMinuteToSecond(result->information_.time, result->goalMinuteTime_, result->goalSecondTime_);

				result->goalTimeText_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->goalTimeText_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, 256, L"%02d m %02d cm", result->goalMinuteTime_, result->goalSecondTime_);
					});
				result->goalTimeText_->SetPSC(Vector3(300.0f, 150.0f, 0.0f), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			}


			//モノを表示
			{
				result->resultGuidanceAttachCountText_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->resultGuidanceAttachCountText_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, 256, L"モノ");
					});
				result->resultGuidanceAttachCountText_->SetPSC(Vector3(300.0f, 50.0f, 0.0f), 1.0f, Vector4(142.0f / 255.0f, 206.0f / 255.0f, 217.0f / 255.0f, 1.0f));
			}

			//吸着したオブジェクトの数を表示
			{
				result->attachableObjectCountText_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->attachableObjectCountText_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, 256, L"%02d 個", result->information_.count);
					});
				result->attachableObjectCountText_->SetPSC(Vector3(300.0f, 0.0f, 0.0f), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
		else 
		{
			// クリアしてないとき

			// ボタンの画像とテキストの設定
			result->buttonSprite_ = std::make_unique<SpriteRender>();
			result->buttonText_ = std::make_unique<FontRender>();
			UIUtil::SetAButtonUI(
				result->buttonSprite_.get(),
				result->buttonText_.get(),
				SPRITE_BUTTON_POS,
				FONT_BUTTON_POS
			);
			result->buttonSprite_->Update();

			// テキストの設定
			result->failureText_ = std::make_unique<FontRender>();
			UIUtil::SetText(result->failureText_.get(), [&](wchar_t* text)
				{
					swprintf_s(text, 256, L"なにやってんだおまえ!!!");
				});
			result->failureText_->SetPSC(Vector3(-780.0f, 0.0f, 0.0f), 3.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}
	void Result::UpdateStep3(Result* result)
	{
		// Jボタンを押したら
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			// 次の処理へ
			result->nextStep_ = Step::Step4;
		}
	}
	void Result::ExitStep3(Result* result)
	{
		result->resultGuidanceSizeText_.reset();
		result->resultSphereSizeText_.reset();
		result->resultGuidanceGoalTime_.reset();
		result->goalTimeText_.reset();
		result->resultGuidanceAttachCountText_.reset();
		result->attachableObjectCountText_.reset();
	}


	void Result::EnterStep4(Result* result)
	{
	}

	void Result::UpdateStep4(Result* result)
	{
		//文字が消えてから2秒たったら
		result->elapsedTime_ += g_gameTime->GetFrameDeltaTime();
		if (result->elapsedTime_ >= FLIGHT_START_DELAY) {
			dynamic_cast<TitleInputSyste*>(result->owner_->inputSystem_)->SetMoveDirection(Vector3(0.0f, 1.0f, 0.5f));	// 斜め奥に行かせたい
			result->nextStep_ = Step::Step5; // 次の処理へ
		}
	}
	void Result::ExitStep4(Result* result)
	{
	}


	void Result::EnterStep5(Result* result)
	{
		result->elapsedTime_ = 0.0f;
	}
	void Result::UpdateStep5(Result* result)
	{
		// フェード処理
		Fade::Get().PlayFade(FadeMode::FadeIn, FADE_OUT_START_TIME,Vector3::Zero);

		if (!Fade::Get().IsPlay()) {
			// 終わり

		}
	}
	void Result::ExitStep5(Result* result)
	{

	}
}




GameScene::GameScene()
{
}


GameScene::~GameScene()
{
	DeleteGO(sphere_);
	DeleteGO(sphereCamera_);
	DeleteGO(stage_);
	DeleteGO(canvas_);
	DeleteGO(inputSystem_);

	CollisionHitManager::Delete();
	LateStageObjectUpdateManager::Get().UnRegisterSphere();
	LateStageObjectUpdateManager::Delete();
	InGameManager::DeleteInstance();
	GameUIManager::DeleteInstance();
	GameTimer::DestroyInstance();
}


bool GameScene::Start()
{
	/* 必要なオブジェクトの作成 */
	sphere_ = NewGO<Sphere>(0,"sphere"); // 塊
	sphereCamera_ = NewGO<SphereCamera>(0, "sphereCamera"); // 塊のカメラ
	stage_ = NewGO<Stage>(0,"stage"); // ステージ
	canvas_ = NewGO<Canvas>(0, "canvas"); // キャンバス
	inputSystem_ = NewGO<InputSystem>(0, "inputSystem"); // 操作用クラスの作成と操作する物の設定


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
	inputSystem_->SetTarget(sphere_); // 操作ターゲットの指定
	sphereCamera_->SetTarget(sphere_); // カメラのターゲットの指定
	GameTimer::Get().SetGameTime(GAME_TIMER_LIMIT); // 時間用クラスにゲームの制限時間を伝える
	GameTimer::Get().Init();
	
	Fade::Get().Stop();

	return true;
}


void GameScene::Update()
{
	switch (gameState_)
	{
		case InGameState::InGame:
		{			
			canvas_->SetTimer(GameTimer::Get().GetRemainingTime()); // 残り時間をUIに伝える
			goalElapsedTime_ += g_gameTime->GetFrameDeltaTime();	// 塊が目標の大きさを達成したときの時間を取得
			
			if (!Fade::Get().IsPlay())
			{
				CalculateFadeAlphaByTime();								// 残り時間が20秒切ってからホワイトアウト
			}

			// クリアしたタイミングの情報を保持する
			if (GameTimer::Get().GetRemainingTime() <= 0.0f) {
				// リザルト準備
				result_ = new _internal::Result(this);
				{
					_internal::ResultInformation info;
					info.time = goalElapsedTime_;			// クリア時間
					info.count = sphere_->GetTotalNum();	// 吸着したオブジェクト数
					info.scale = sphere_->GetRadius();		// 半径
					result_->SetInformation(info);
				}

				gameState_ = InGameState::InGameFinish; // 次の処理へ移行
			}

			break;
		}
		case InGameState::InGameFinish:
		{
			// 不要になったものの削除
			DeleteGO(canvas_);
			canvas_ = nullptr;
			DeleteGO(sphereCamera_);

			// リザルト用カメラの生成と設定
			sphereCamera_ = NewGO<ResultCamera>(0, "resultCamera"); // リザルト用のカメラを作成
			sphereCamera_->SetCameraActive(false); // カメラを操作不能にする
			sphereCamera_->Update(); // カメラの最終座標を更新
			Vector3 sphereCameraPos = sphereCamera_->GetPosition(); // カメラの座標を保存


			//if (sphere_->CheakGoalSize()) // クリアしているなら
			{
				sphere_->SetPosition(Vector3(0.0f, 3000.0f, 0.0f)); // 塊を上空に移動
				sphereCamera_->SetPosition(sphereCameraPos); // リザルト用のカメラの設定
			}

			result_->Start();

			gameState_ = InGameState::Result; // 次の処理に進む
			break;
		}
		case InGameState::Result:
		{
			result_->Update();
			break;
		}
	}
	CollisionHitManager::Get().Update();
	LateStageObjectUpdateManager::Get().Update();
	GameTimer::Get().Update();
}


void GameScene::Render(RenderContext& rc)
{
	if (result_) {
		result_->Render(rc);
	}
}



bool GameScene::RequestID(uint32_t& id, float& waitTime)
{
	if (isNextScene_) {
		id = ResultScene::ID();
		waitTime = 5.0f;
		return true;
	}
	return false;
}


void GameScene::CalculateFadeAlphaByTime()
{
	if (GameTimer::Get().GetRemainingTime() > 20.0f) { return; } // 残り時間が20秒切っていない場合は処理を返す

	// 20秒から10秒までの処理
	if (GameTimer::Get().GetRemainingTime() >= 10.0f)
	{
		Fade::Get().PlayFade(FadeMode::FadeOut, FADE_START_TIME);
		// 10秒でα値を0.3まであげたい
	}

	// 10秒からタイムアップまで
	//else if (GameTimer::Get().GetRemainingTime() <= 10.0f)
	//{
	//	Fade::Get().PlayFade(FadeMode::FadeOut, FADE_START_TIME);
	//	// 残り10秒でα値を1.0まで上げる
	//}
}


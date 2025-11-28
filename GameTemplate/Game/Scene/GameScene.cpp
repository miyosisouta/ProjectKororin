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


namespace
{
	constexpr float FADE_OUT_START_TIME = 1.0f;			// フェードアウトが始まるまでの時間
	constexpr float FLIGHT_START_DELAY = 2.5f;			// オブジェクトが空へ飛ぶまでの時間
	constexpr const float GAME_TIMER_LIMIT = 20.0f;		// ゲーム時間
	constexpr const int METERS_TO_CENTIMETERS = 100;	// メートルとセンチメートルを分ける

	// フェードの境界時間を定数として定義
	const float FADE_START_TIME = 20.0f; // フェード開始 (α = 0.0f)
	const float FADE_MID_TIME = 10.0f; // 中間点 (α = 0.3f)
	const float FADE_END_TIME = 0.0f;  // フェード終了 (α = 1.0f)

	const float STAGE_DURATION = 10.0f; // 各ステージの長さ (10秒)
	const float ALPHA_MAX_FIRST_HALF = 0.3f;  // ステージ1の最大アルファ値
	const float ALPHA_RANGE_SECOND_HALF = 0.7f;  // ステージ2のアルファ値の増加幅 (1.0f - 0.3f)

	/** タイムを秒に変換 */
	int CalcMinuteToSecond(int clearTime, int& clearTimeMinute, int& clearTimeSecond)
	{
		clearTimeMinute = clearTime / 60; // 分を返す
		clearTimeSecond = clearTime % 60; // 秒を返す
		return 1;
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
	

	// フェード終わり
	Fade::Get().SetAlpha(0.0f);

	return true;
}


void GameScene::Update()
{
	switch (gameState_)
	{
		case InGameState::InGame:
		{
			canvas_->SetTimer(GameTimer::Get().GetRemainingTime()); // 残り時間をUIに伝える
			CalculateFadeAlphaByTime(); // 残り時間が20秒切ってからホワイトアウト
			goalElapsedTime_ += g_gameTime->GetFrameDeltaTime(); // 塊が目標の大きさを達成したときの時間を取得

			// クリアしたタイミングの情報を保持する
			if (!resultInformation_.isSetting && GameTimer::Get().GetRemainingTime() <= 0.0f) {
				resultInformation_.time = goalElapsedTime_;
				resultInformation_.count = sphere_->GetTotalNum();;
				resultInformation_.scale = sphere_->GetRadius();
				resultInformation_.isSetting = true;

				gameState_ = InGameState::InGameFinish;
			}

			break;
		}
		case InGameState::InGameFinish:
		{
			gameState_ = InGameState::Result; // 次の処理に進む
			DeleteGO(canvas_); // 不要になったオブジェクトを削除
			canvas_ = nullptr; // 不要になったものをnullにする

			// オブジェクトの生成・設定
			sphere_->SetPosition(Vector3(0.0f,3000.0f,0.0f)); // 塊を上空に移動

			sphereCamera_->SetCameraActive(false); // カメラを操作不能にする
			sphereCamera_->Update(); // カメラの最終座標を更新
			Vector3 sphereCameraPos = sphereCamera_->GetPosition(); // カメラの座標を保存
			DeleteGO(sphereCamera_); // 現在のカメラを削除
			sphereCamera_ = NewGO<ResultCamera>(0, "resultCamera"); // リザルト用のカメラを作成
			sphereCamera_->SetPosition(sphereCameraPos); // リザルト用のカメラの設定

			if (!sphere_->CheakGoalSize()) // クリアしていない場合
			{
				// 背景用オブジェクトの作成・設定
				blackOutObject_.Init("Assets/modelData/stage/result/blackOutObject.tkm"); 
				blackOutObject_.SetPosition(Vector3(0.0f, 2500.0f, -100.0f));
				blackOutObject_.SetScale(10.0f);
				blackOutObject_.Update();

				// 塊は描画しない
				sphere_->SetIsDraw(false);
				// オブジェクトお非表示
				stage_->SetAllVisible(false);
				stage_->SetVisibleAttackObject(false);
			}
			break;
		}
		case InGameState::Result:
		{
			switch (step_)
			{
				case Step1:
				{
					// 今のInputSystemを削除し、タイトル用のInputSystemを作成
					DeleteGO(inputSystem_);
					inputSystem_ = NewGO<TitleInputSyste>(0, "inputSystem");
					inputSystem_->SetTarget(sphere_);
					step_ = Step2;
					break;
				}
				case Step2:
				{
					// 1秒かけてホワイトインさせる
					elapsedTime_ += g_gameTime->GetFrameDeltaTime();
					const float alphaValue = 1.0f - min(elapsedTime_ / FADE_OUT_START_TIME, 1.0f);
					Fade::Get().SetAlpha(alphaValue);

					// 1秒たったら次の処理段階へ移行
					if (elapsedTime_ >= FADE_OUT_START_TIME) {
						step_ = Step3;
						elapsedTime_ = 0.0f;
					}
					break;
				}
				case Step3:
				{	
					if (sphere_->CheakGoalSize()) { // クリアのとき
						// 大きさを表示
						{
							wchar_t text[256];
							swprintf_s(text, 256, L"大きさ");
							resultGuidanceSizeText_.SetText(text);
							resultGuidanceSizeText_.SetPSC(Vector3(300.0f, 350.0f, 0.0f), 1.0f, Vector4(180.0f / 255.0f, 241.0f / 255.0f, 249.0f / 255.0f, 1.0f));
						}

						// 塊の大きさの表示
						{
							const int radiusMetersText = resultInformation_.scale / METERS_TO_CENTIMETERS; // メートルを算出
							const int radiusCentimeters = (int)resultInformation_.scale % METERS_TO_CENTIMETERS; // センチメートルを算出
							wchar_t text[256];
							swprintf_s(text, 256, L"%02d m %02d cm", radiusMetersText, radiusCentimeters);
							resultSphereSizeText_.SetText(text);
							resultSphereSizeText_.SetPSC(Vector3(300.0f, 300.0f, 0.0f), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
						}


						// 目標達成を表示
						{
							wchar_t text[256];
							swprintf_s(text, 256, L"達成時間");
							resultGuidanceGoalTime_.SetText(text);
							resultGuidanceGoalTime_.SetPSC(Vector3(300.0f, 200.0f, 0.0f), 1.0f, Vector4(65.0f / 255.0f, 130.0f / 255.0f, 250.0f / 255.0f, 1.0f));
						}

						// 目標サイズ達成時間の表示
						{
							CalcMinuteToSecond(resultInformation_.time, goalMinuteTime_, goalSecondTime_);
							wchar_t text[256];
							swprintf_s(text, 256, L"%02d m %02d cm", goalMinuteTime_, goalSecondTime_);
							goalTimeText_.SetText(text);
							goalTimeText_.SetPSC(Vector3(300.0f, 150.0f, 0.0f), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
						}


						//モノを表示
						{
							wchar_t text[256];
							swprintf_s(text, 256, L"モノ");
							resultGuidanceAttachCountText_.SetText(text);
							resultGuidanceAttachCountText_.SetPSC(Vector3(300.0f, 50.0f, 0.0f), 1.0f, Vector4(142.0f / 255.0f, 206.0f / 255.0f, 217.0f / 255.0f, 1.0f));
						}

						//吸着したオブジェクトの数を表示
						{
							wchar_t text[256];
							swprintf_s(text, 256, L"%02d 個", resultInformation_.count);
							attachableObjectCountText_.SetText(text);
							attachableObjectCountText_.SetPSC(Vector3(300.0f, 0.0f, 0.0f), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
						}
					}

					else // クリアしてないとき
					{
						// 背景用画像を視界外から出してくる
						

						// テキストの設定
						wchar_t text[256];
						swprintf_s(text, 256, L"なにやってんだおまえ!!!");
						failureText_.SetText(text);
						failureText_.SetPSC(Vector3(-800.0f, 50.0f, 0.0f), 3.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
					}

					// ボタンの設定
					buttonSprite_.Init("Assets/Sprite/Result/Button.DDS", 256, 256);
					buttonSprite_.SetPSM(Vector3(-300.0f, -300.0f, 0.0f), 1.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
					buttonSprite_.Update();
					wchar_t text[256];
					swprintf_s(text, 256, L"をおしてね");
					buttonText_.SetText(text);
					buttonText_.SetPSC(Vector3(-250.0f, -300.0f, 0.0f), 1.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
					isResultTextRender_ = true; // テキストをDraw

					// Jボタンを押したら
					if (g_pad[0]->IsTrigger(enButtonA)) 
					{
						isResultTextRender_ = false; // テキストを削除
						step_ = Step4; // 次の処理へ
					}
					break;
				}
				case Step4:
				{
					//文字が消えてから2秒たったら
					elapsedTime_ += g_gameTime->GetFrameDeltaTime();
					if (elapsedTime_ >= FLIGHT_START_DELAY) {
						dynamic_cast<TitleInputSyste*>(inputSystem_)->SetMoveDirection(Vector3(0.0f, 1.0f, 0.5f));	// 斜め奥に行かせたい
						step_ = Step5; // 次の処理へ
						elapsedTime_ = 0.0f;// 経過時間をリセット
					}
					break;
				}
				case Step5:
				{
					Fade::Get().SetAlpha(0.0f);
					Fade::Get().SetColor(0.0f);
					elapsedTime_ += g_gameTime->GetFrameDeltaTime();

					// ブラックアウト
					const float fadeAlphaValue = min(elapsedTime_ / FADE_OUT_START_TIME, 1.0f);
					Fade::Get().SetAlpha(fadeAlphaValue);

					if (elapsedTime_ >= FADE_OUT_START_TIME) {
						// 終わり
					}
					break;
				}
			}



			break;
		}
	}
	CollisionHitManager::Get().Update();
	LateStageObjectUpdateManager::Get().Update();
	GameTimer::Get().Update();
}


void GameScene::Render(RenderContext& rc)
{
	if (isResultTextRender_ && sphere_->CheakGoalSize())
	{
		resultGuidanceSizeText_.Draw(rc);
		resultSphereSizeText_.Draw(rc);
		resultGuidanceGoalTime_.Draw(rc);
		goalTimeText_.Draw(rc);
		resultGuidanceAttachCountText_.Draw(rc);
		attachableObjectCountText_.Draw(rc);
	}
	else if (isResultTextRender_ && !sphere_->CheakGoalSize()) {
		blackOutObject_.Draw(rc);
		failureText_.Draw(rc);
	}
	//buttonSprite_.Draw(rc);
	//buttonText_.Draw(rc);
}


bool GameScene::RequestID(uint32_t& id)
{
	if (isNextScene_) {
		id = ResultScene::ID();
		return true;
	}
	return false;
}


void GameScene::CalculateFadeAlphaByTime()
{
	if (GameTimer::Get().GetRemainingTime() > 20.0f) { return; } // 残り時間が20秒切っていない場合は処理を返す

	float currentTime = GameTimer::Get().GetRemainingTime(); // 現在の残り時間を取得


	// 20秒から10秒までの処理
	if (currentTime >= 10.0f)
	{
		float calcAlpha = (FADE_START_TIME - currentTime) / STAGE_DURATION; // 経過時間による比率の計算(最大1.0)
		float fadeAlpha = calcAlpha * ALPHA_MAX_FIRST_HALF; //最大アルファ値(0.3f)を乗算し、最終的なα値を計算
		whiteOutAlpha = fadeAlpha;
	}

	// 10秒からタイムアップまで
	else if (currentTime <= 10.0f)
	{
		float calcAlpha = (FADE_MID_TIME - currentTime) / STAGE_DURATION; // 経過時間による比率の計算(最大1.0)
		float fadeAlpha = ALPHA_MAX_FIRST_HALF + calcAlpha * ALPHA_RANGE_SECOND_HALF;//前半のMax増加幅と後半の増加幅(0.7f)を乗算した値を加算し、最終的なα値を計算
		whiteOutAlpha = fadeAlpha;
	}
	Fade::Get().SetAlpha(whiteOutAlpha); // データを送る
}


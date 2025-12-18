#include "stdafx.h"
#include "GameScene.h"
#include "SphereInputSystem.h"
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
#include "Scene/TitleScene.h"
#include "UI/Canvas.h"
#include "UI/UIBase.h"
#include "UI/Util.h"
#include "Util/MessageText.h"
#include "Util/InputDetection.h"


namespace
{
	constexpr const float FADE_OUT_START_TIME = 2.0f;			// フェードアウトが始まるまでの時間
	constexpr const float FLIGHT_START_DELAY = 1.5;				// オブジェクトが空へ飛ぶまでの時間
	constexpr const float SPHERE_TO_DELETE_TIME = 5.0f;			// オブジェクトが空へ飛ぶまでの時間
	constexpr const float GAME_TIMER_LIMIT = 10.0f;				// ゲーム時間
	constexpr const float SPHERE_RESULT_LERP_UP_TIME = 2.0f;	// リザルト時塊がふわふわと線形補間により行う上昇にかける時間
	constexpr const float SPHERE_RESULT_LERP_DOWN_TIME = 3.0f;	// リザルト時塊がふわふわと線形補間により行う下降にかける時間
	constexpr const int METERS_TO_CENTIMETERS = 100;			// メートルとセンチメートルを分ける
	constexpr const uint8_t SET_CAN_NUMBER_CHARACTERS = 256;	// 設定可能な文字数
	static const Vector3 SPRITE_BUTTON_POS = Vector3(-250.0f, -300.0f, 0.0f);			// ボタンの画像の座標
	static const Vector3 FONT_BUTTON_POS = Vector3(-80.0f, -300.0f, 0.0f);				// ボタンテキストの画像の座標
	static const Vector3 FONT_FAILER_TEXTS_POS = Vector3(150.0f, 300.0f, 0.0f);			// クリア失敗時のリザルトでのテキスト位置
	static const Vector3 BLACK_OBJECT_INIT_POS = Vector3(0.0f, 1500.0f, -1000.0f);		// 黒い背景用オブジェクトの最初の位置
	static const Vector3 BLACK_OBJECT_LAST_POS = Vector3(0.0f, 0.0f, -1000.0f);			// 黒い背景用オブジェクトの最終的な位置
	static const Vector3 SPHERE_RESULT_CLEAR_POS = Vector3(0.0f, 3000.0f, 0.0f);		// クリアしているときの塊の座標
	static const Vector3 SPHERE_RESULT_FAILER_MIN_POS = Vector3(0.0f, 100.0f, -200.0f);	// クリアしていないときの塊が一番下にいる座標
	static const Vector3 SPHERE_RESULT_FAILER_MAX_POS = Vector3(0.0f, 200.0f, -200.0f);	// クリアしていないときの塊が一番上にいる座標
	static const Vector3 SPHERE_RESULT_FAILER_LAST_POS = Vector3(0.0f, 0.0f, 180.0f);		// クリアしていないときの塊が最終的にいる位置座標

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
		// 状態を変更
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

		// 画像の更新
		{
			// Canvasの更新
			if (instructionButtonSprite_) {
				instructionButtonSprite_->Update();
			}
			// タイトルへの画像更新
			if (titleTransitionSprite_) {
				titleTransitionSprite_->Update();
			}
		}

		// 入力判定の状態を更新
		owner_->inputDetection_->UpdateTriggerState();
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
			if (textWindowSprite_) {
				textWindowSprite_->Draw(rc);
			}
			if (failureTexts_[currentSentenceIndex]) {
				failureTexts_[currentSentenceIndex]->Draw(rc);
			}
			if (instructionButtonSprite_) {
				instructionButtonSprite_->Render(rc);
			}
			if (breakScreenSprite_) {
				breakScreenSprite_->Render(rc);
			}
			if (titleTransitionSprite_) {
				titleTransitionSprite_->Render(rc);
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
		// step6
		setting(Step::Step6, EnterStep6, UpdateStep6, ExitStep6);
	}


	void Result::EnterStep1(Result* result)
	{
		// 今のInputSystemを削除し、タイトル用のInputSystemを作成
		DeleteGO(result->owner_->sphereInputSystem_);
		result->owner_->sphereInputSystem_ = NewGO<TitleInputSyste>(0, "inputSystem");
		result->owner_->sphereInputSystem_->SetTarget(result->owner_->sphere_);
		result->owner_->sphere_->SetPlayable(false);


		// クリアしていない場合
		if (!result->owner_->sphere_->CheakGoalSize())
		{
			// 背景用オブジェクトの作成・設定
			result->blackOutObject_ = std::make_unique<ModelRender>();
			result->blackOutObject_->Init("Assets/modelData/stage/result/blackOutObject_second.tkm");
			result->blackOutObject_->SetPosition(BLACK_OBJECT_INIT_POS);
			result->blackOutObject_->SetScale(Vector3(25.0f, 15.0f, 0.5f));
			result->blackOutObject_->Update();

			// 塊の設定
			result->owner_->sphere_->SetIsDraw(false);
			result->owner_->sphere_->SetPosition(SPHERE_RESULT_FAILER_MIN_POS);
			result->owner_->sphere_->Update();

			// オブジェクト非表示
			result->owner_->stage_->SetAllVisible(false);
			result->owner_->stage_->SetVisibleAttackObject(false);

			// ディレクションンライトのパラメーター設定
			g_sceneLight->SetDirectionLight(0, Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));

			// スカイキューブを非アクティブに
			result->owner_->skyCube_->Deactivate();
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
		result->calclerpValue_.InitCalcTime(5.0f);
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
		/******** クリアのとき ******/

		if (result->owner_->sphere_->CheakGoalSize())
		{
			// 大きさを表示
			{
				result->resultGuidanceSizeText_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->resultGuidanceSizeText_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, SET_CAN_NUMBER_CHARACTERS, L"大きさ");
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
						swprintf_s(text, SET_CAN_NUMBER_CHARACTERS, L"%02d m %02d cm", radiusMetersText, radiusCentimeters);
					});
				result->resultSphereSizeText_->SetPSC(Vector3(300.0f, 300.0f, 0.0f), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			}


			// 目標達成を表示
			{
				result->resultGuidanceGoalTime_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->resultGuidanceGoalTime_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, SET_CAN_NUMBER_CHARACTERS, L"達成時間");
					});
				result->resultGuidanceGoalTime_->SetPSC(Vector3(300.0f, 200.0f, 0.0f), 1.0f, Vector4(65.0f / 255.0f, 130.0f / 255.0f, 250.0f / 255.0f, 1.0f));
			}

			// 目標サイズ達成時間の表示
			{
				CalcMinuteToSecond(result->information_.time, result->goalMinuteTime_, result->goalSecondTime_);

				result->goalTimeText_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->goalTimeText_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, SET_CAN_NUMBER_CHARACTERS, L"%02d 分 %02d 秒", result->goalMinuteTime_, result->goalSecondTime_);
					});
				result->goalTimeText_->SetPSC(Vector3(300.0f, 150.0f, 0.0f), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			}


			//モノを表示
			{
				result->resultGuidanceAttachCountText_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->resultGuidanceAttachCountText_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, SET_CAN_NUMBER_CHARACTERS, L"モノ");
					});
				result->resultGuidanceAttachCountText_->SetPSC(Vector3(300.0f, 50.0f, 0.0f), 1.0f, Vector4(142.0f / 255.0f, 206.0f / 255.0f, 217.0f / 255.0f, 1.0f));
			}

			//吸着したオブジェクトの数を表示
			{
				result->attachableObjectCountText_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->attachableObjectCountText_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, SET_CAN_NUMBER_CHARACTERS, L"%02d 個", result->information_.count);
					});
				result->attachableObjectCountText_->SetPSC(Vector3(300.0f, 0.0f, 0.0f), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}


		/************** クリアしてないとき *********/

		else 
		{
			// リープ先の設定
			result->SetUpToLerp(SPHERE_RESULT_FAILER_MIN_POS, SPHERE_RESULT_FAILER_MAX_POS);

			// リープで使用する時間の設定
			result->calclerpValue_.InitCalcTime(1.5f);

			// 塊を描画
			result->owner_->sphere_->SetIsDraw(true);

			// 文字の設定
			for (int i = 0; i < 5; ++i) 
			{
				// フォントレンダーのユニークポインタの作成
				result->failureTexts_[i] = std::make_unique<FontRender>();

				// MessageTextからテキストをセット、タイプをEnglishに設定
				result->failureTexts_[i]->SetText(GetMessageText(i, MessageType::MessageScene::GameFailure));

				// テキストの初期設定
				result->failureTexts_[i]->SetPSC(
					FONT_FAILER_TEXTS_POS,
					0.7f,
					Vector4::White
				);
			}


			// 画像の表示
			result->instructionButtonSprite_ = new UICanvas;
			result->instructionIcon_ = result->instructionButtonSprite_->CreateUI<UIIcon>();
			result->instructionIcon_->Initialize("Assets/sprite/Result/instructionButton.DDS", 128, 128, Vector3(800.0f, 180.0f, 0.0f), Vector3(0.3f, 0.3f, 0.3f), Quaternion::Identity);
			// ここからイージング設定
			auto scaleAnimation = std::make_unique<UIColorAnimation>();
			scaleAnimation->SetParameter(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 0.0f), 2.0f, EasingType::EaseInOut, LoopMode::PingPong);
			result->instructionIcon_->SetUIAnimation(std::move(scaleAnimation));
			result->instructionIcon_->PlayAnimation();


			// テキストウィンドウの画像の設定
			result->textWindowSprite_ = std::make_unique<SpriteRender>();
			result->textWindowSprite_->Init("Assets/sprite/Result/textWindow.DDS", 256, 256);
			result->textWindowSprite_->SetPSM(
				Vector3(500.0f, 250.0f, 0.0f),
				3.0f,
				Vector4(1.0f, 1.0f, 1.0f, 1.0f)
			);
			result->textWindowSprite_->Update();

			// 画像の更新
			result->instructionButtonSprite_->Update();

		}
	}
	void Result::UpdateStep3(Result* result)
	{
		// 塊を上下にふよふよさせる
		{
			result->elapsedTime_ += g_gameTime->GetFrameDeltaTime();
			// リープ処理
			//  初期位置を0.5とし0.0～1.0を何度も増減する、
			const float lerpValue = (sinf(result->elapsedTime_) + 1.0f) * 0.5f;

			Vector3 currentPos = result->owner_->sphere_->GetPosition(); // 今いる座標
			currentPos.Lerp(lerpValue, result->sphereResultGoalPos, result->sphereResultInitPos); // 移動先の座標を線形補完
			result->owner_->sphere_->SetPosition(currentPos); // 移動先のポジションを設定
			result->owner_->sphere_->Update();
		}


		// 指示画像の点滅
		{
			result->instructionButtonSprite_->Update();
		}


		// Jボタンを押したら、次の文への変更
		{
			if (result->owner_->inputDetection_->IsTriggerButtonA())
			{
				// 次の文への移行
				result->currentSentenceIndex++;
				if (result->currentSentenceIndex >= 5)
				{
					// 
					result->currentSentenceIndex = 4;

					// 次の処理へ
					result->nextStep_ = Step::Step4;

				}
			}
		}
		
	}
	void Result::ExitStep3(Result* result)
	{
		if (result->owner_->sphere_->CheakGoalSize())
		{
			result->resultGuidanceSizeText_.reset();
			result->resultSphereSizeText_.reset();
			result->resultGuidanceGoalTime_.reset();
			result->goalTimeText_.reset();
			result->resultGuidanceAttachCountText_.reset();
			result->attachableObjectCountText_.reset();
			result->buttonText_.reset();
		}
		else 
		{
			for (int i = 0; i <= 4; ++i) { result->failureTexts_[i].reset(); } // テキストを削除

			result->instructionIcon_->isDraw = false;
			result->textWindowSprite_.reset();
			result->buttonSprite_.reset();
		}
		result->elapsedTime_ = 0.0f;
	}


	void Result::EnterStep4(Result* result)
	{
		// リープで使用する時間の設定
		result->calclerpValue_.InitCalcTime(FLIGHT_START_DELAY);
		// リープさせるための座標を設定
		result->SetUpToLerp(result->owner_->sphere_->GetPosition(), SPHERE_RESULT_FAILER_LAST_POS);
	}

	void Result::UpdateStep4(Result* result)
	{
		result->elapsedTime_ += g_gameTime->GetFrameDeltaTime();
		if (result->elapsedTime_ >= FLIGHT_START_DELAY) // 2秒経過したら
		{
			// クリアしている場合
			if (result->owner_->sphere_->CheakGoalSize())
			{
				dynamic_cast<TitleInputSyste*>(result->owner_->sphereInputSystem_)->SetMoveDirection(Vector3(0.0f, 1.0f, 0.5f));	// 斜め奥に行かせたい
				if (result->elapsedTime_ >= SPHERE_TO_DELETE_TIME) { return; } // 5秒たつまで次のシーンには移行しない
			}

			// クリア失敗している場合
			else
			{
				//テキストが消えてから時間計算
				const float lerpValue = result->calclerpValue_.CalcUpdate(FLIGHT_START_DELAY);

				Vector3 spherePos = result->owner_->sphere_->GetPosition(); // 今の座標を取得
				spherePos.Lerp(lerpValue, result->sphereResultInitPos, result->sphereResultGoalPos); // 線形補間
				result->owner_->sphere_->SetPosition(spherePos); // 移動先のポジションを設定
				result->owner_->sphere_->Update(); // 座標を更新

				Vector3 deltaPos = result->sphereResultGoalPos - result->owner_->sphere_->GetPosition();
				if (deltaPos.Length() >= 1.2f) { return; } // 塊が移動しきるまで次のシーンへ移行しない
			}

			// 画面外に行った塊のクラスと関係するクラスの削除
			DeleteGO(result->owner_->sphere_);
			DeleteGO(result->owner_->sphereCamera_);
			DeleteGO(result->owner_->sphereInputSystem_);

			result->nextStep_ = Step::Step5; // 次の処理へ
		}
	}
	void Result::ExitStep4(Result* result)
	{
		result->elapsedTime_ = 0.0f;
	}


	void Result::EnterStep5(Result* result)
	{
		// クリアしている場合
		if (result->owner_->sphere_->CheakGoalSize())
		{
		}

		else 
		{
			// カメラレンズが割れた画像
			{
				result->breakScreenSprite_ = new UICanvas;
				auto* icon = result->breakScreenSprite_->CreateUI<UIIcon>();
				icon->Initialize("Assets/sprite/Result/breakScreen.DDS", 1920.0f, 1080.0f, Vector3(0.0f, 0.0f, 0.0f), Vector3::One, Quaternion::Identity);
			}

			// 「タイトル」へ画像
			{
				// 画像の初期設定
				result->titleTransitionSprite_ = new UICanvas;
				result->titleTransitionWindowIcon_ = result->titleTransitionSprite_->CreateUI<UIIcon>();
				result->titleTransitionWindowIcon_->Initialize("Assets/sprite/Result/titleTransitionButton.DDS", 512.0f, 256.0f, Vector3(-100.0f, -250.0f, 0.0f), Vector3::One, Quaternion::Identity);

				// ここからイージング設定
				auto scaleAnimation = std::make_unique<UIScaleAnimation>();
				scaleAnimation->SetParameter(Vector3(1.0f), Vector3(0.75f), 8.0f, EasingType::EaseInOut, LoopMode::PingPong);
				result->titleTransitionSprite_->SetUIAnimation(std::move(scaleAnimation));
				result->titleTransitionWindowIcon_->PlayAnimation();

				auto colorAnimation = std::make_unique<UIColorAnimation>();
				colorAnimation->SetParameter(Vector4(0.0f, 0.75f, 0.0, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), 8.0f, EasingType::EaseInOut, LoopMode::PingPong);
				result->titleTransitionWindowIcon_->SetUIAnimation(std::move(colorAnimation));
				result->titleTransitionWindowIcon_->PlayAnimation();
				
				// Aボタン画像
				result->buttonAIcon_ = result->titleTransitionSprite_->CreateUI<UIIcon>();
				result->buttonAIcon_->Initialize("Assets/sprite/UI/Button_A.DDS", 100.0f, 100.0f, Vector3(270.0f, -250.0f, 0.0f), Vector3::One, Quaternion::Identity);

				// 画像を更新
				result->titleTransitionSprite_->Update();
			}
		}
	}
	void Result::UpdateStep5(Result* result)
	{
		if (!g_pad[0]->IsTrigger(enButtonA)) { return; } // Aボタンを押していない場合処理を返す

		if (result->owner_->sphere_->CheakGoalSize()) 
		{
			// フェード処理(フェードアウト・2秒・白)
			Fade::Get().PlayFade(FadeMode::FadeOut);
		}

		else
		{
			// 画像更新
			result->titleTransitionSprite_->Update();

			//フェード開始(フェードアウト・2秒・黒)
			Fade::Get().PlayFade(FadeMode::FadeOut, FADE_OUT_START_TIME,fadeColorPreset::BLACK_COLOR_RGB);
		}
		result->nextStep_ = Step::Step6;

	}
	void Result::ExitStep5(Result* result)
	{
	}


	void Result::EnterStep6(Result* result)
	{
	}
	void Result::UpdateStep6(Result* result)
	{
		if (!Fade::Get().IsPlay())
		{
			// 次の処理へ
			result->nextStep_ = Step::Max;
			result->owner_->isNextScene_ = true;
		}
	}
	void Result::ExitStep6(Result* result)
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
	DeleteGO(sphereInputSystem_);

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
	sphereInputSystem_ = NewGO<SphereInputSystem>(0, "inputSystem"); // 操作用クラスの作成と操作する物の設定
	inputDetection_ = new InputDetection; // 入力判定用クラス

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
	sphereInputSystem_->SetTarget(sphere_); // 操作ターゲットの指定
	sphereCamera_->SetTarget(sphere_); // カメラのターゲットの指定
	GameTimer::Get().SetGameTime(GAME_TIMER_LIMIT); // 時間用クラスにゲームの制限時間を伝える
	GameTimer::Get().Init();
	
	Fade::Get().Stop();

	g_sceneLight->SetDirectionLight(0, Vector3(1.0f, -1.0f, -1.0f), Vector3(0.8f));
	g_sceneLight->SetAmbinet(Vector3(0.8f));

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
			
			if (sphere_->CheakGoalSize()) // クリアしているなら 
			{
				Vector3 sphereCameraPos = sphereCamera_->GetPosition(); // カメラの座標を保存		
				sphere_->SetPosition(SPHERE_RESULT_CLEAR_POS); // 塊を上空に移動
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

	// スカイキューブをプレイヤー追従にする
	// 例外でFindGOする
	skyCube_ = FindGO<SkyCube>("skyCube");
	if (skyCube_)
	{
		Vector3 skyCubePosition = sphere_->GetPosition();
		skyCubePosition.y = 0.0f;
		skyCube_->SetPosition(skyCubePosition);
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
		id = TitleScene::ID();
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
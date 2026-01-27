#include "stdafx.h"
#include "GameScene.h"
#include "SphereInputSystem.h"
#include "SphereCamera.h"
#include "SpacePartitioning.h"
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
#include "Sound/SoundManager.h"


namespace
{
	/********* 共通の定数 *********/
	constexpr const float FADE_OUT_START_TIME = 2.0f;								// フェードアウトが始まるまでの時間
	constexpr const float GAME_TIMER_LIMIT = 210.0f;								// ゲーム時間
	static const Vector3 RESULT_CAMERA_POS = Vector3(0.0f, 0.0f, 0.0f);				// クリア時のカメラの座標
	constexpr const uint8_t SENTENCE_COUNT_MAX = 4;									// テキストの最大文の数
	constexpr const uint8_t MAX_SENTENCE_NUM = 5;									// スタートイベント用テキストの最大文の数


	/********* インゲーム開始時のスタートイベント用の定数 *********/
	constexpr const float FADEIN_TIME_START_EVENT = 3.0f;							// スタートイベント終了時のフェードインにかける時間
	constexpr const float TIME_VALUE = 1.0f;
	static const Vector3 START_EVENT_TEXT_POS = Vector3(-380.0f, 380.0f, 0.0f);		// スタートイベント用テキストの位置

	/********* ゲームクリア時の定数 *********/
	constexpr const float SPHERE_TO_DELETE_TIME = 5.0f;								// オブジェクトが空へ飛ぶまでの時間
	constexpr const float CLEAR_SPHERE_ROTATION_SPEED = 100.0f;						// 塊の回転速度
	constexpr const int METERS_TO_CENTIMETERS = 100;								// メートルとセンチメートルを分ける
	constexpr const uint8_t HIGHEST_RATED_BORDER = 0;								// 最高評価ボーダーライン
	constexpr const uint16_t SET_CAN_NUMBER_CHARACTERS = 256;						// 設定可能な文字数
	static const Vector3 CLAER_SPHERE_INIT_POS = Vector3(0.0f, 0.0f, -10.0f);		// 塊の初期座標
	static const Vector3 CLAER_SPHERE_LAST_POS = Vector3(120.0f, 0.0f, -120.0f);	// 塊の移動後の座標

	/********* ゲームオーバー時の定数 *********/
	constexpr const float FLIGHT_START_DELAY = 1.5f;								// 塊がカメラの後方へ飛ぶまでの時間
	static const Vector3 FAILER_FONT_TEXTS_POS = Vector3(150.0f, 330.0f, 0.0f);		// クリア失敗時のリザルトでのテキスト位置
	static const Vector3 BLACK_OBJECT_INIT_POS = Vector3(0.0f, 1500.0f, -1000.0f);	// 黒い背景用オブジェクトの最初の位置
	static const Vector3 BLACK_OBJECT_LAST_POS = Vector3(0.0f, 0.0f, -1000.0f);		// 黒い背景用オブジェクトの最終的な位置
	static const Vector3 FAILER_SPHERE_MIN_POS = Vector3(0.0f, 100.0f, -200.0f);	// クリアしていないときの塊が一番下にいる座標
	static const Vector3 FAILER_SPHERE_MAX_POS = Vector3(0.0f, 200.0f, -200.0f);	// クリアしていないときの塊が一番上にいる座標
	static const Vector3 FAILER_SPHERE_LAST_POS = Vector3(0.0f, 0.0f, 180.0f);		// クリアしていないときの塊が最終的にいる位置座標


	/********* フェードの境界時間を定数として定義 *********/
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
	Result::Result(GameScene* owner)
		: owner_(owner)
	{
	}
	Result::~Result()
	{
	}

	void Result::Start()
	{
		// 不要になったものの削除
		DeleteGO(owner_->canvas_);
		owner_->canvas_ = nullptr;
		DeleteGO(owner_->sphereCamera_);

		// リザルト用カメラの生成
		owner_->sphereCamera_ = NewGO<ResultCamera>(0, "resultCamera"); // リザルト用のカメラを作成
		owner_->sphereCamera_->SetCameraActive(false); // カメラを操作不能にする
		owner_->sphereCamera_->SetPosition(RESULT_CAMERA_POS); // カメラの座標を設定
		owner_->sphereCamera_->Update(); // カメラの最終座標を更新

	}
	void Result::Update()
	{
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
		if (textWindowSprite_) {
			textWindowSprite_->Draw(rc);
		}
		if (instructionButtonSprite_) {
			instructionButtonSprite_->Render(rc);
		}
	}


	/**************************************************************  クリア時の処理  ****************************************************************************/

	void Result::CommonEnterStep1(Result* result)
	{
		// 今のInputSystemを削除し、タイトル用のInputSystemを作成
		DeleteGO(result->owner_->sphereInputSystem_);
		result->owner_->sphereInputSystem_ = NewGO<TitleInputSyste>(0, "inputSystem");
		result->owner_->sphereInputSystem_->SetTarget(result->owner_->sphere_);
		result->owner_->sphere_->SetPlayable(false);


		// フェード処理
		Fade::Get().PlayFade(FadeMode::FadeIn, FADE_OUT_START_TIME);
	}
	void Result::CommonUpdateStep1(Result* result)
	{
	}
	void Result::CommonExitStep1(Result* result)
	{
	}


	void Result::CommonEnterStep2(Result* result)
	{

	}
	void Result::CommonUpdateStep2(Result* result)
	{
	}
	void Result::CommonExitStep2(Result* result)
	{

	}


	void Result::CommonEnterStep3(Result* result)
	{

	}
	void Result::CommonUpdateStep3(Result* result)
	{
	}
	void Result::CommonExitStep3(Result* result)
	{
		result->elapsedTime_ = 0.0f;
	}


	void Result::CommonEnterStep4(Result* result)
	{

	}
	void Result::CommonUpdateStep4(Result* result)
	{

	}
	void Result::CommonExitStep4(Result* result)
	{
		result->elapsedTime_ = 0.0f;
	}


	void Result::CommonEnterStep5(Result* result)
	{

	}
	void Result::CommonUpdateStep5(Result* result)
	{
		if (!g_pad[0]->IsTrigger(enButtonA)) { return; } // Aボタンを押していない場合処理を返す
	}
	void Result::CommonExitStep5(Result* result)
	{
	}


	void Result::CommonEnterStep6(Result* result)
	{
	}
	void Result::CommonUpdateStep6(Result* result)
	{
		if (!Fade::Get().IsPlay())
		{
			// 次の処理へ
			result->owner_->isNextScene_ = true;
		}
	}
	void Result::CommonExitStep6(Result* result)
	{
	};



	/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
	/**************************************************************  クリア時の処理  *****************************************************************************/
	/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/


	ClearResult::ClearResult(GameScene* owner)
		: Result(owner)
	{
		// リストに登録するための関数定義
		{
			auto setting = [&](ClearStep::Enum step, EnterFunc enter, UpdateFunc update, ExitFunc exit)
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
					return;
				};

			// step1
			setting(ClearStep::Step1, EnterStep1, UpdateStep1, ExitStep1);
			// step2
			setting(ClearStep::Step2, EnterStep2, UpdateStep2, ExitStep2);
			// step3
			setting(ClearStep::Step3, EnterStep3, UpdateStep3, ExitStep3);
			// step4
			setting(ClearStep::Step4, EnterStep4, UpdateStep4, ExitStep4);
			// step5
			setting(ClearStep::Step5, EnterStep5, UpdateStep5, ExitStep5);
			// step6
			setting(ClearStep::Step6, EnterStep6, UpdateStep6, ExitStep6);
		}

	}
	ClearResult::~ClearResult()
	{
	}

	void ClearResult::Start()
	{
		// 共通初期化処理呼び出し
		Result::Start();

		nextStep_ = ClearStep::Step1;
		auto& currentState = stepList_[currentStep_];
		currentState.enter(this);
	}
	void ClearResult::Update()
	{
		// 状態を変更
		{
			auto& currentState = stepList_[currentStep_];
			if (nextStep_ != ClearStep::Invalid && nextStep_ != currentStep_)
			{
				currentState.exit(this);
				currentState = stepList_[nextStep_];
				currentStep_ = nextStep_;
				currentState.enter(this);
			}
			currentState.update(this);
		}

		Result::Update();
	}
	void ClearResult::Render(RenderContext& rc)
	{
		// 共通で使うものの描画
		Result::Render(rc);

		// ClearResultでのみ使うものの描画
		{
			if (scoreWindow_) {
				scoreWindow_->Render(rc);
			}
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
			if (clearTexts_[currentSentenceIndex_]) {
				clearTexts_[currentSentenceIndex_]->Draw(rc);
			}
			if (normalRatingStamp_) {
				normalRatingStamp_->Render(rc);
			}
			if (goodRatingStamp_) {
				goodRatingStamp_->Render(rc);
			}
		}

	}


	void ClearResult::EnterStep1(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonEnterStep1(result);


		// インゲームにて「塊に吸着しているオブジェクトを描画できる」ようフラグを立てている
		// オブジェクト全て非表示→個人(吸着しているオブジェクト)のフラグが優先されて描画がされる
		SpacePartitioning::GetInstance()->OffRender();
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////result->owner_->stage_->SetVisibleAll(false);

		// 経過時間の初期化
		result->elapsedTime_ = 0.0f;
		// 塊の移動時間
		result->calclerpValue_.InitCalcTime(3.0f);

		// 塊の初期座標の設定
		{
			result->owner_->sphere_->SetPosition(CLAER_SPHERE_INIT_POS);
			result->owner_->sphere_->Update();
		}
	}
	void ClearResult::UpdateStep1(ClearResult* result)
	{
		Result::CommonUpdateStep1(result);

		// フェードイン終了時次の処理へ移行
		if (!Fade::Get().IsPlay()) {
			result->nextStep_ = ClearStep::Step2;
		}
	}
	void ClearResult::ExitStep1(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonExitStep1(result);
	}


	void ClearResult::EnterStep2(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonEnterStep2(result);

	}
	void ClearResult::UpdateStep2(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonUpdateStep2(result);


		// 塊の常時回転
		{
			//// 1フレームの経過時間を取得
			//float orbMoveSpeed = CLEAR_SPHERE_ROTATION_SPEED * g_gameTime->GetFrameDeltaTime();
		}

		// 塊の移動処理
		{
			const float lerpValue = result->calclerpValue_.CalcUpdate();

			Vector3 spherePos = result->owner_->sphere_->GetPosition(); // 今の座標を取得
			spherePos.Lerp(lerpValue, CLAER_SPHERE_INIT_POS, CLAER_SPHERE_LAST_POS); // 線形補間
			result->owner_->sphere_->SetPosition(spherePos); // 移動先のポジションを設定
			result->owner_->sphere_->Update(); // 座標を更新
		}

		// 次のステップへの移行
		{
			Vector3 deltaPos = CLAER_SPHERE_LAST_POS - result->owner_->sphere_->GetPosition();
			if (deltaPos.Length() <= 1.2f) {
				result->nextStep_ = ClearStep::Step3;
			}
		}
	}
	void ClearResult::ExitStep2(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonExitStep2(result);
	}


	void ClearResult::EnterStep3(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonEnterStep3(result);

		// 画像の表示
		{
			//指示ボタンの画像設定
			{
				result->instructionButtonSprite_ = new UICanvas;
				result->instructionIcon_ = result->instructionButtonSprite_->CreateUI<UIIcon>();
				result->instructionIcon_->Initialize("Assets/sprite/Result/instructionButton.DDS", 128, 128, Vector3(800.0f, 180.0f, 0.0f), Vector3(0.3f, 0.3f, 0.3f), Quaternion::Identity);
				// ここからイージング設定
				auto scaleAnimation = std::make_unique<UIColorAnimation>();
				scaleAnimation->SetParameter(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 0.0f), 2.0f, EasingType::EaseInOut, LoopMode::PingPong);
				result->instructionIcon_->SetUIAnimation(std::move(scaleAnimation));
				result->instructionIcon_->PlayAnimation();
			}

			// テキストウィンドウの画像の設定
			{
				result->textWindowSprite_ = std::make_unique<SpriteRender>();
				result->textWindowSprite_->Init("Assets/sprite/Result/textWindow.DDS", 256, 350);
				result->textWindowSprite_->SetPSM(
					Vector3(500.0f, 300.0f, 0.0f),
					3.0f,
					Vector4(1.0f, 1.5f, 1.0f, 1.0f)
				);
				result->textWindowSprite_->Update(); // 更新
			}
		}

		// テキストの設定
		{
			// 文字の設定
			for (int i = 0; i < 5; ++i)
			{
				// フォントレンダーのユニークポインタの作成
				result->clearTexts_[i] = std::make_unique<FontRender>();

				// MessageTextからテキストをセット、タイプをEnglishに設定
				result->clearTexts_[i]->SetText(GetMessageText(i, MessageType::MessageScene::GameClear_Good));

				// テキストの初期設定
				result->clearTexts_[i]->SetPSC(
					FAILER_FONT_TEXTS_POS,
					0.7f,
					Vector4::White
				);
			}
		}
		// 画像の更新
		result->instructionButtonSprite_->Update();


	}
	void ClearResult::UpdateStep3(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonUpdateStep3(result);

		// 指示画像の点滅
		{
			result->instructionButtonSprite_->Update();
		}
		// テキストの遷移
		{
			if (result->owner_->inputDetection_->IsTriggerButtonA())
			{
				result->currentSentenceIndex_++;
				if (result->currentSentenceIndex_ > SENTENCE_COUNT_MAX)
				{
					result->currentSentenceIndex_ = SENTENCE_COUNT_MAX;// 文の最大数を固定
					result->nextStep_ = ClearStep::Step4;	// 次のステップへ
				}
			}
		}
	}
	void ClearResult::ExitStep3(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonExitStep3(result);

		// テキストの削除
		for (int i = 0; i < 5; ++i)
		{
			result->clearTexts_[i].reset();
		}
		result->instructionIcon_->isDraw = false;
		result->textWindowSprite_ = nullptr;
	}


	void ClearResult::EnterStep4(ClearResult* result)
	{
		// 共通処理
		Result::CommonEnterStep4(result);

		// スコアウィンドウの設定
		{
			result->scoreWindow_ = new UICanvas;
			result->scoreWindowIcon_ = result->scoreWindow_->CreateUI<UIIcon>();
			result->scoreWindowIcon_->Initialize("Assets/sprite/Result/scoreWindow.DDS", 768, 768, Vector3(350.0f, 50.0f, 0.0f), Vector3(1.8f, 1.5f, 1.0f), Quaternion::Identity);
			// ここからイージング設定
			auto scaleAnimation = std::make_unique<UIColorAnimation>();
			scaleAnimation->SetParameter(Vector4(1.0f, 1.0f, 1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 0.8f), 2.0f, EasingType::EaseInOut, LoopMode::PingPong);
			result->scoreWindowIcon_->SetUIAnimation(std::move(scaleAnimation));
			result->scoreWindowIcon_->PlayAnimation();
		}

		// テキストの表示設定
		{
			// 大きさを表示
			{
				result->resultGuidanceSizeText_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->resultGuidanceSizeText_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, SET_CAN_NUMBER_CHARACTERS, L"大きさ");
					});
				result->resultGuidanceSizeText_->SetPSC(Vector3(30.0f, 350.0f, 0.0f), 1.0f, Vector4(180.0f / 255.0f, 241.0f / 255.0f, 249.0f / 255.0f, 1.0f));
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
				result->resultSphereSizeText_->SetPSC(Vector3(100.0f, 300.0f, 0.0f), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 0.0f));
			}


			// 目標達成を表示
			{
				result->resultGuidanceGoalTime_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->resultGuidanceGoalTime_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, SET_CAN_NUMBER_CHARACTERS, L"達成時間");
					});
				result->resultGuidanceGoalTime_->SetPSC(Vector3(30.0f, 150.0f, 0.0f), 1.0f, Vector4(180.0f / 255.0f, 241.0f / 255.0f, 249.0f / 255.0f, 1.0f));
			}

			// 目標サイズ達成時間の表示
			{
				CalcMinuteToSecond(result->information_.time, result->goalMinuteTime_, result->goalSecondTime_);

				result->goalTimeText_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->goalTimeText_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, SET_CAN_NUMBER_CHARACTERS, L"%02d 分 %02d 秒", result->goalMinuteTime_, result->goalSecondTime_);
					});
				result->goalTimeText_->SetPSC(Vector3(100.0f, 100.0f, 0.0f), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 0.0f));
			}


			//モノを表示
			{
				result->resultGuidanceAttachCountText_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->resultGuidanceAttachCountText_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, SET_CAN_NUMBER_CHARACTERS, L"モノ");
					});
				result->resultGuidanceAttachCountText_->SetPSC(Vector3(30.0f, -50.0f, 0.0f), 1.0f, Vector4(180.0f / 255.0f, 241.0f / 255.0f, 249.0f / 255.0f, 1.0f));
			}

			//吸着したオブジェクトの数を表示
			{
				result->attachableObjectCountText_ = std::make_unique<FontRender>();
				UIUtil::SetText(result->attachableObjectCountText_.get(), [&](wchar_t* text)
					{
						swprintf_s(text, SET_CAN_NUMBER_CHARACTERS, L"%02d 個", result->information_.count);
					});
				result->attachableObjectCountText_->SetPSC(Vector3(100.0f, -100.0f, 0.0f), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}
		// テキストのアルファ値を上げる時間の設定
		result->calclerpValue_.InitCalcTime(5.0f);
	}
	void ClearResult::UpdateStep4(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonUpdateStep4(result);

		// スコア表示されてから少し待つ
		float value = result->calclerpValue_.CalcUpdate();
		if (value >= 1.0f)
		{
			result->nextStep_ = ClearStep::Step5;
		}
	}
	void ClearResult::ExitStep4(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonExitStep4(result);


	}


	void ClearResult::EnterStep5(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonEnterStep5(result);

		result->scaleRate_ = result->information_.scale / METERS_TO_CENTIMETERS;

		if (result->scaleRate_ >= HIGHEST_RATED_BORDER)
		{
			// 評価用印鑑の設定
			result->goodRatingStamp_ = new UICanvas;
			result->goodRatingStampIcon_ = result->goodRatingStamp_->CreateUI<UIIcon>();
			result->goodRatingStampIcon_->Initialize("Assets/sprite/Result/sphereLank_Good.DDS", 512, 512, Vector3(400.0f, -100.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Quaternion::Identity);
			// ここからイージング設定
			auto scaleAnimation = std::make_unique<UIScaleAnimation>();
			scaleAnimation->SetParameter(Vector3(1.0f), Vector3(0.6f), 4.0f, EasingType::EaseInOut, LoopMode::PingPong);
			result->goodRatingStampIcon_->SetUIAnimation(std::move(scaleAnimation));
			result->goodRatingStampIcon_->PlayAnimation();
		}
		else
		{
			// 評価用印鑑の設定
			result->normalRatingStamp_ = new UICanvas;
			result->normalRatingStampIcon_ = result->normalRatingStamp_->CreateUI<UIIcon>();
			result->normalRatingStampIcon_->Initialize("Assets/sprite/Result/sphereLank_Normal.DDS", 512, 512, Vector3(400.0f, -100.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Quaternion::Identity);
			// ここからイージング設定
			auto scaleAnimation = std::make_unique<UIScaleAnimation>();
			scaleAnimation->SetParameter(Vector3(1.0f), Vector3(0.6f), 4.0f, EasingType::EaseInOut, LoopMode::PingPong);
			result->normalRatingStampIcon_->SetUIAnimation(std::move(scaleAnimation));
			result->normalRatingStampIcon_->PlayAnimation();
		}
		// スタンプを押すアニメーションの終わる時間を設定
		result->calclerpValue_.InitCalcTime(2.5f);
	}
	void ClearResult::UpdateStep5(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonUpdateStep5(result);

		// 時間経過
		const float value = result->calclerpValue_.CalcUpdate();
		if (value >= 1.0f)
		{
			SoundManager::Get().PlaySE(enSoundKind_RateStamp);
			result->nextStep_ = ClearStep::Step6;
		}
	}
	void ClearResult::ExitStep5(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonExitStep5(result);
	}


	void ClearResult::EnterStep6(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonEnterStep6(result);
	}
	void ClearResult::UpdateStep6(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonUpdateStep6(result);

		//dynamic_cast<TitleInputSyste*>(result->owner_->sphereInputSystem_)->SetMoveDirection(Vector3(0.0f, 1.0f, 0.5f));	// 斜め奥に行かせたい
		//if (result->elapsedTime_ >= SPHERE_TO_DELETE_TIME) { return; } // 5秒たつまで次のシーンには移行しない
	}
	void ClearResult::ExitStep6(ClearResult* result)
	{
		// 共通処理の呼び出し
		Result::CommonExitStep6(result);

		// テキストを破棄
		result->resultGuidanceSizeText_.reset();
		result->resultSphereSizeText_.reset();
		result->resultGuidanceGoalTime_.reset();
		result->goalTimeText_.reset();
		result->resultGuidanceAttachCountText_.reset();
		result->attachableObjectCountText_.reset();
	}


	/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------  失敗時の処理  ---------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/


	void FailureResult::EnterStep1(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonEnterStep1(result);

		// 背景用オブジェクトの作成・設定
		{
			result->blackOutObject_ = std::make_unique<ModelRender>();
			result->blackOutObject_->Init("Assets/modelData/stage/result/blackOutObject_second.tkm");
			result->blackOutObject_->SetPosition(BLACK_OBJECT_INIT_POS);
			result->blackOutObject_->SetScale(Vector3(25.0f, 15.0f, 0.5f));
			result->blackOutObject_->Update();
		}

		// 塊の設定
		{
			result->owner_->sphere_->SetIsDraw(false);
			result->owner_->sphere_->SetPosition(FAILER_SPHERE_MIN_POS);
			result->owner_->sphere_->Update();
		}

		// オブジェクト非表示
		SpacePartitioning::GetInstance()->OffRender();

		// ディレクションンライトのパラメーター設定
		g_sceneLight->SetDirectionLight(0, Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));

		// スカイキューブを非アクティブ
		result->owner_->skyCube_->Deactivate();
	}
	void FailureResult::UpdateStep1(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonUpdateStep1(result);


		// ホワイトイン完了した場合
		if (!Fade::Get().IsPlay())
		{
			result->isMoveBlackOutObject_ = true;
			result->nextStep_ = FailureStep::Step2;
		}
	}
	void FailureResult::ExitStep1(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonExitStep1(result);
	}


	void FailureResult::EnterStep2(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonEnterStep2(result);

		// リープで使用する時間の設定
		result->calclerpValue_.InitCalcTime(5.0f);
	}
	void FailureResult::UpdateStep2(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonUpdateStep2(result);

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
			else
			{
				result->nextStep_ = FailureStep::Step3;
			}
		}
	}
	void FailureResult::ExitStep2(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonExitStep2(result);
	}


	void FailureResult::EnterStep3(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonEnterStep3(result);

		// リープ先の設定
		result->SetUpToLerp(FAILER_SPHERE_MIN_POS, FAILER_SPHERE_MAX_POS);

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
				FAILER_FONT_TEXTS_POS,
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
	void FailureResult::UpdateStep3(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonUpdateStep3(result);

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
				result->currentSentenceIndex_++;
				if (result->currentSentenceIndex_ > SENTENCE_COUNT_MAX)
				{
					result->currentSentenceIndex_ = SENTENCE_COUNT_MAX;// 文の最大数を固定
					result->nextStep_ = FailureStep::Step4;	// 次のステップへ
				}
			}
		}
	}
	void FailureResult::ExitStep3(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonExitStep3(result);

		for (int i = 0; i < 5; ++i) { result->failureTexts_[i].reset(); } // テキストを削除

		result->instructionIcon_->isDraw = false;
		result->textWindowSprite_.reset();
	}


	void FailureResult::EnterStep4(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonEnterStep4(result);

		// リープで使用する時間の設定
		result->calclerpValue_.InitCalcTime(FLIGHT_START_DELAY);
		// リープさせるための座標を設定
		result->SetUpToLerp(result->owner_->sphere_->GetPosition(), FAILER_SPHERE_LAST_POS);
	}
	void FailureResult::UpdateStep4(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonUpdateStep4(result);

		//塊がカメラに向かって飛んでいく動き
		{
			const float lerpValue = result->calclerpValue_.CalcUpdate();

			Vector3 spherePos = result->owner_->sphere_->GetPosition(); // 今の座標を取得
			spherePos.Lerp(lerpValue, result->sphereResultInitPos, result->sphereResultGoalPos); // 線形補間
			result->owner_->sphere_->SetPosition(spherePos); // 移動先のポジションを設定
			result->owner_->sphere_->Update(); // 座標を更新
		}

		// 次のステップへの移行
		{
			Vector3 hoge = result->owner_->sphere_->GetPosition();
			Vector3 deltaPos = result->sphereResultGoalPos - result->owner_->sphere_->GetPosition();
			if (deltaPos.Length() <= 1.2f) {
				result->nextStep_ = FailureStep::Step5;	// 次のステップへ
			}
		}

	}
	void FailureResult::ExitStep4(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonExitStep4(result);

		//result->elapsedTime_ += g_gameTime->GetFrameDeltaTime();
		//if (result->elapsedTime_ >= FLIGHT_START_DELAY) // 2秒経過したら
		//{
			// 画面外に行った塊のクラスと関係するクラスの削除
		DeleteGO(result->owner_->sphere_);
		DeleteGO(result->owner_->sphereCamera_);
		DeleteGO(result->owner_->sphereInputSystem_);
		//}
	}


	void FailureResult::EnterStep5(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonEnterStep5(result);

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
	void FailureResult::UpdateStep5(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonUpdateStep5(result);
		// 画像更新
		result->titleTransitionSprite_->Update();

		if (g_pad[0]->IsPress(enButtonA))
		{
			//フェード開始(フェードアウト・2秒・黒)
			Fade::Get().PlayFade(FadeMode::FadeOut, FADE_OUT_START_TIME, fadeColorPreset::BLACK_COLOR_RGB);
			result->nextStep_ = FailureStep::Step6;	// 次のステップへ
		}
	}
	void FailureResult::ExitStep5(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonExitStep5(result);
	}


	void FailureResult::EnterStep6(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonEnterStep6(result);
	}
	void FailureResult::UpdateStep6(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonUpdateStep6(result);
	}
	void FailureResult::ExitStep6(FailureResult* result)
	{
		// 共通処理呼び出し
		Result::CommonExitStep6(result);
	}


	FailureResult::FailureResult(GameScene* owner)
		: Result(owner)
	{
		// リストに登録するための関数定義
		{
			auto setting = [&](FailureStep::Enum step, EnterFunc enter, UpdateFunc update, ExitFunc exit)
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
					return;
				};

			// step1
			setting(FailureStep::Step1, EnterStep1, UpdateStep1, ExitStep1);
			// step2
			setting(FailureStep::Step2, EnterStep2, UpdateStep2, ExitStep2);
			// step3
			setting(FailureStep::Step3, EnterStep3, UpdateStep3, ExitStep3);
			// step4
			setting(FailureStep::Step4, EnterStep4, UpdateStep4, ExitStep4);
			// step5
			setting(FailureStep::Step5, EnterStep5, UpdateStep5, ExitStep5);
			// step6
			setting(FailureStep::Step6, EnterStep6, UpdateStep6, ExitStep6);
		}

	}
	FailureResult::~FailureResult()
	{
	}

	void FailureResult::Start()
	{
		// 共通初期化処理を呼び出し
		Result::Start();

		// 次のステップに移行
		nextStep_ = FailureStep::Step1;
		auto& currentState = stepList_[currentStep_];
		currentState.enter(this);
	}
	void FailureResult::Update()
	{
		// 状態を変更
		{
			auto& currentState = stepList_[currentStep_];
			if (nextStep_ != FailureStep::Invalid && nextStep_ != currentStep_)
			{
				currentState.exit(this);
				currentState = stepList_[nextStep_];
				currentStep_ = nextStep_;
				currentState.enter(this);
			}
			currentState.update(this);
		}

		Result::Update();
	}
	void FailureResult::Render(RenderContext& rc)
	{
		// 共通で使うものの描画
		Result::Render(rc);

		// FailureResultでのみ使う物の描画
		{
			if (blackOutObject_ && isMoveBlackOutObject_) {
				blackOutObject_->Draw(rc);
			}

			if (failureTexts_[currentSentenceIndex_]) {
				failureTexts_[currentSentenceIndex_]->Draw(rc);
			}
			if (breakScreenSprite_) {
				breakScreenSprite_->Render(rc);
			}
			if (titleTransitionSprite_) {
				titleTransitionSprite_->Render(rc);
			}
		}
	}


	/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------  インゲームが始まった際のイベントの処理  -------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/

	StartEvent::StartEvent(GameScene* owner)
		: owner_(owner)
	{
		// リストに登録するための関数定義
		{
			auto setting = [&](StartEventStep::Enum step, EnterFunc enter, UpdateFunc update, ExitFunc exit)
				{

					if (startEventStepList_[step].enter == nullptr) {
						startEventStepList_[step].enter = enter;
					}
					if (startEventStepList_[step].update == nullptr) {
						startEventStepList_[step].update = update;
					}
					if (startEventStepList_[step].exit == nullptr) {
						startEventStepList_[step].exit = exit;
					}
					return;
				};

			// step1
			setting(StartEventStep::Step1, EnterStep1, UpdateStep1, ExitStep1);
			// step2
			setting(StartEventStep::Step2, EnterStep2, UpdateStep2, ExitStep2);
		}
	}
	StartEvent::~StartEvent()
	{
	}

	void StartEvent::Start()
	{
		nextStep_ = StartEventStep::Step1;
		auto& currentState = startEventStepList_[currentStep_];
		currentState.enter(this);
	}
	void StartEvent::Update()
	{
		if (currentStep_ == StartEventStep::Max) { return; }
		// 状態を変更
		{
			auto& currentState = startEventStepList_[currentStep_];
			if (nextStep_ != StartEventStep::Invalid && nextStep_ != currentStep_)
			{
				currentState.exit(this);
				if (nextStep_ == StartEventStep::Max)
				{
					currentStep_ = nextStep_;
					return;
				}

				currentState = startEventStepList_[nextStep_];
				currentStep_ = nextStep_;
				currentState.enter(this);
			}
			currentState.update(this);
		}
	}
	void StartEvent::Render(RenderContext& rc)
	{
		if (startEventTextWindow_) {
			startEventTextWindow_->Render(rc);
		}
		if (instructionButtonSprite_) {
			instructionButtonSprite_->Render(rc);
		}
		if (texts_[currentSentenceNum_]) {
			texts_[currentSentenceNum_]->Draw(rc);
		}
	}



	void StartEvent::EnterStep1(StartEvent* owner)
	{
		// 文字の設定
		{
			for (int i = 0; i < MAX_SENTENCE_NUM; ++i)
			{
				// フォントレンダーのユニークポインタの作成
				owner->texts_[i] = std::make_unique<FontRender>();

				// MessageTextからテキストをセット、タイプをEnglishに設定
				owner->texts_[i]->SetText(GetMessageText(i, MessageType::MessageScene::GameStart));

				// テキストの初期設定
				owner->texts_[i]->SetPSC(
					START_EVENT_TEXT_POS,
					0.8f,
					Vector4::White
				);
			}
		}

		// テキストウィンドウの画像の設定と指示画像の表示
		{
			// テキストウィンドウの画像
			owner->startEventTextWindow_ = new UICanvas;
			owner->startEventTextWindowIcon_ = owner->startEventTextWindow_->CreateUI<UIIcon>();
			owner->startEventTextWindowIcon_->Initialize("Assets/sprite/Result/textWindow.DDS", 1024.0f, 768.0f, Vector3(50.0f, 320.0f, 0.0f), Vector3::One, Quaternion::Identity);
			// ここからイージング設定
			auto colorWindowAnimation = std::make_unique<UIColorAnimation>();
			colorWindowAnimation->SetParameter(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 0.0f), 1.5f, EasingType::EaseIn, LoopMode::Once);
			owner->startEventTextWindowIcon_->SetUIAnimation(std::move(colorWindowAnimation));
			// animationの再生はテキスト読み終わった後に行う


			// 指示画像
			owner->instructionButtonSprite_ = new UICanvas;
			owner->instructionIcon_ = owner->instructionButtonSprite_->CreateUI<UIIcon>();
			owner->instructionIcon_->Initialize("Assets/sprite/Result/instructionButton.DDS", 128, 128, Vector3(400.0f, 240.0f, 0.0f), Vector3(0.3f, 0.3f, 0.3f), Quaternion::Identity);
			// ここからイージング設定
			auto colorButtonAnimation = std::make_unique<UIColorAnimation>();
			colorButtonAnimation->SetParameter(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 0.0f), 0.5f, EasingType::EaseInOut, LoopMode::PingPong);
			owner->instructionIcon_->SetUIAnimation(std::move(colorButtonAnimation));
			owner->instructionIcon_->PlayAnimation();


			// 画像の更新
			owner->instructionButtonSprite_->Update();
		}
	}
	void StartEvent::UpdateStep1(StartEvent* owner)
	{
		// 更新
		{
			owner->owner_->inputDetection_->UpdateTriggerState(); // 入力
			owner->instructionIcon_->Update(); // 画像のアニメーション用
		}

		// Jボタンを押したら、次の文への変更
		{
			if (owner->owner_->inputDetection_->IsTriggerButtonA())
			{
				// 次の文への移行
				owner->currentSentenceNum_++;
				if (owner->currentSentenceNum_ >= MAX_SENTENCE_NUM)
				{
					owner->currentSentenceNum_ = SENTENCE_COUNT_MAX;// 文の最大数を固定
					owner->nextStep_ = StartEventStep::Step2;
				}
			}
		}

	}
	void StartEvent::ExitStep1(StartEvent* owner)
	{
		// テキストの破棄
		for (int i = 0; i < MAX_SENTENCE_NUM; ++i) {
			owner->texts_[i].reset();
		}

		// 指示画像の破棄
		delete owner->instructionButtonSprite_;
		owner->instructionButtonSprite_ = nullptr;
	}

	void StartEvent::EnterStep2(StartEvent* owner)
	{
		owner->startEventTextWindowIcon_->PlayAnimation(); // テキストウィンドウのフェードイン
		owner->calcValue_.InitCalcTime(FADEIN_TIME_START_EVENT);
	}

	void StartEvent::UpdateStep2(StartEvent* owner)
	{
		if (owner->startEventTextWindowIcon_) { owner->startEventTextWindowIcon_->Update(); } // 画像のアニメーション用

		if (owner->calcValue_.CalcUpdate() >= TIME_VALUE && !owner->isFinished_)
		{
			// フェードイン開始
			Fade::Get().PlayFade(FadeMode::FadeIn, FADEIN_TIME_START_EVENT);
			// インゲームスタートイベントの処理を終わる
			owner->isFinished_ = !owner->isFinished_;

			// テキストウィンドウを削除する
			delete owner->startEventTextWindow_;
			owner->startEventTextWindow_ = nullptr;
		}
	}

	void StartEvent::ExitStep2(StartEvent* owner)
	{

	}
}

/********************** StartEventを作成 ************************/

StartEventObject::StartEventObject()
{
}

StartEventObject::~StartEventObject()
{
	delete startEvent_;
	startEvent_ = nullptr;
}

bool StartEventObject::Start()
{
	startEvent_ = new _internal::StartEvent(owner_);
	startEvent_->Start();
	return true;
}
void StartEventObject::Update()
{
	if (!active_) { return; }// アクティブでないなら処理しない

	// スタートイベントの更新
	startEvent_->Update();
}
void StartEventObject::Render(RenderContext& rc)
{
	if (startEvent_) {
		startEvent_->Render(rc);
	}
}


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------  GameSceneの処理  ---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------------------*/


GameScene::GameScene()
{
	// スタートイベントがないなら作成・開始
	// 実行順番を下げることでUIが上に表示されるようにする
	startEventObject_ = NewGO<StartEventObject>(1, "startEventObject");
	startEventObject_->SetOwner(this);
}
GameScene::~GameScene()
{
	DeleteGO(sphere_);
	DeleteGO(sphereCamera_);
	DeleteGO(canvas_);
	DeleteGO(sphereInputSystem_);
	DeleteGO(startEventObject_); // 不要なら削除（またはメンバ変数として持ち続けるなら維持）

	CollisionHitManager::Delete();
	LateStageObjectUpdateManager::Get().UnRegisterSphere();
	LateStageObjectUpdateManager::Delete();
	InGameManager::DeleteInstance();
	GameUIManager::DeleteInstance();
	GameTimer::DestroyInstance();
	SpacePartitioning::DeleteInstance();

}

bool GameScene::Start()
{
	/* 必要なオブジェクトの作成 */
	sphere_ = NewGO<Sphere>(0, "sphere"); // 塊
	sphereCamera_ = NewGO<SphereCamera>(0, "sphereCamera"); // 塊のカメラ
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
	SpacePartitioning::CreateInstance();
	SpacePartitioning::GetInstance()->UpdateStart();


	// インゲームスタートイベント実行のためコメントアウト
	/*Fade::Get().Stop();*/

	g_sceneLight->SetDirectionLight(0, Vector3(0.5f, -1.0f, -1.0f), Vector3(0.5f));
	g_sceneLight->SetAmbinet(Vector3(0.8f));

	return true;
}


void GameScene::Update()
{
	switch (gameState_)
	{
	case InGameState::InGameStartEvent:
	{
		// イベントが終わったらゲーム本編へ
		if (startEventObject_->GetStartEvent()->IsFinished()) {
			// スタートイベントの削除
			{
				if (startEventObject_) {
					startEventObject_->SetActive(false);
				}
			}

			// ゲーム開始時にしたい処理
			{
				canvas_ = NewGO<Canvas>(0, "canvas"); // キャンバス

				sphereInputSystem_->SetTarget(sphere_);			// 操作ターゲットの指定
				sphereCamera_->SetTarget(sphere_);				// カメラのターゲットの指定
				GameTimer::Get().SetGameTime(GAME_TIMER_LIMIT);					// 時間用クラスにゲームの制限時間を伝える
				GameTimer::Get().Init();										// ゲーム時間を残り時間に設定
			}
			gameState_ = InGameState::InGame;
		}
		break;
	}
	case InGameState::InGame:
	{
		canvas_->SetTimer(GameTimer::Get().GetRemainingTime()); // 残り時間をUIに伝える
		goalElapsedTime_ += g_gameTime->GetFrameDeltaTime();	// 塊が目標の大きさを達成したときの時間を取得

		if (!Fade::Get().IsPlay())
		{
			CalculateFadeAlphaByTime();								// 残り時間が20秒切ってからホワイトアウト
		}

		// 残り時間が無くなった場合
		// クリアしたタイミングの情報を保持する
		if (GameTimer::Get().GetRemainingTime() <= 0.0f)
		{
			// リザルト準備
			// クリアの場合、クリアリザルトの処理が呼ばれる
			if (sphere_->CheakGoalSize())
			{
				auto* result = new _internal::ClearResult(this);
				_internal::ClearResult::ResultInformation info;
				info.time = goalElapsedTime_;			// クリア時間
				info.count = sphere_->GetTotalNum();	// 吸着したオブジェクト数
				info.scale = sphere_->GetRadius();		// 半径
				result->SetInformation(info);

				result_ = result;
			}

			// 失敗の場合、失敗リザルトの処理が呼ばれる
			else
			{
				auto* result = new _internal::FailureResult(this);
				result_ = result;
			}

			gameState_ = InGameState::InGameFinish; // 次の処理へ移行
		}

		break;
	}
	case InGameState::InGameFinish:
	{
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
	SpacePartitioning::GetInstance()->Update();

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
	else if (GameTimer::Get().GetRemainingTime() <= 10.0f)
	{
		Fade::Get().PlayFade(FadeMode::FadeOut, FADE_START_TIME);
		// 残り10秒でα値を1.0まで上げる
	}
}

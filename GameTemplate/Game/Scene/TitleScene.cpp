#include "stdafx.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SphereInputSystem.h"
#include "Actor/Sphere/Sphere.h"
#include "Core/Fade.h"
#include "Collision/CollisionManager.h"
#include "Sound/SoundManager.h"
#include "UI/Util.h"
#include "Util/Util.h"

// @todo for test
#include "UI/UIBase.h"


namespace
{
	/* ゲームタイトル画像 */
	constexpr float GAME_NAME_SPRITE_WIDTH = 1024.0f; // 画像サイズ：横
	constexpr float GAME_NAME_SPRITE_HEIGHT = 512.0f; // 画像サイズ：縦
	static const Vector3 GAME_NAME_SPRITE_POS = Vector3(0.0f, 250.0f, 0.0f); // 座標

	/* Press Aの画像 */
	constexpr float PRESS_A_SPRITE_WIDTH = 512.0f; // 画像サイズ : 横
	constexpr float PRESS_A_SPRITE_HEIGHT = 256.0f; // 画像サイズ : 縦
	static const Vector3 PRESS_A_SPRITE_POS = Vector3(0.0f, -250.0f, 0.0f); // 座標
	static const Vector4 PRESS_A_SPRITE_ANIMATION_COLOR_BEFORE = Vector4(1.0f,1.0f,1.0f,1.0f); // アニメーション前の色
	static const Vector4 PRESS_A_SPRITE_ANIMATION_COLOR_AFTER = Vector4(1.0f,1.0f,1.0f,0.1f); // アニメーション後の色
	constexpr float PRESS_A_ANIMATION_TIME = 3.0f; // アニメーションにかかる時間

	/* ラープ */
	constexpr float LERP_CULC_ADD = 1.0f; // リープ計算時の加算
	constexpr float LERP_CULC_MULT = 0.5f; // リープ計算時の乗算

	/* その他 */
	constexpr float NEXT_SCENE_ANIMATION_TIME = 2.0f; // アニメーション時間
	constexpr float SKYCUBE_INIT_POS = 0.0f; // スカイキューブの初期座標
	constexpr uint8_t PRIOLITY_ZERO = 0; // 優先度
	static const Vector3 SPHERE_INIT_POS = Vector3(0.0f, 1.0f, -50.0f); // 塊の初期座標 
	constexpr float NEAR_AREA = 0.01f; // 近距離のエリア
	constexpr float MIDDLE_AREA = 0.1f; // 中距離エリア
	constexpr float FAR_AREA = 0.5f; // 遠距離エリア
	constexpr uint8_t DIRECTION_LIGHT_NOM = 0; // ディレクションライトのナンバー
	static const Vector3 DIRECTION_LIGHT_DIR = Vector3(0.0f, -0.5f, 1.0f); // ディレクションライトの方向
	static const Vector3 DIRECTION_LIGHT_COLOR = Vector3(1.2f); // ディレクションライトの色
	static const Vector3 AMBIENT_LIGHT = Vector3(0.6f); // 環境光
	static const Vector3 CAMERA_POS = Vector3(0.0f, 100.0f, -200.0f); // カメラ座標
	static const Vector3 CAMERA_TARGET_POS = Vector3(0.0f, 50.0f, 0.0f); // 注視点の座標
	constexpr uint8_t GAME_PAD_NUM = 0; // ゲームパッドの番号
	constexpr float SCENE_TRANSITION_WAIT_TIME = 5.0f; // シーン遷移時に必要な時間
}

TitleScene::TitleScene()
{
	CollisionHitManager::CreateInstance();

	if (Fade::Get().GetMode() == FadeMode::FadeOut)
	{
		Fade::Get().PlayFade(FadeMode::FadeIn);
	}
}

TitleScene::~TitleScene()
{
	// 当たり判定
	CollisionHitManager::Delete();

	// 塊
	DeleteGO(sphere_);
	// 操作システム
	DeleteGO(inputSystem_);

	// タイトルの画像
	if (titleGameNameCanvas_)
	{
		delete titleGameNameCanvas_;
		titleGameNameCanvas_ = nullptr;
	}

	// ボタンの画像
	if (pressButtonCanvas_) {
		delete pressButtonCanvas_;
		pressButtonCanvas_ = nullptr;
	}
}

bool TitleScene::Start()
{
	// 必要な機能のNewGO
	sphere_ = NewGO<Sphere>(PRIOLITY_ZERO, "sphere");
	inputSystem_ = NewGO<TitleInputSyste>(PRIOLITY_ZERO, "inputSystem");


	// ゲームループ時、スカイキューブがfalseの場合アクティブにする
	auto* skyCube = FindGO<SkyCube>("skyCube");
	if (!skyCube->IsActive()) {
		skyCube->Activate();
		Vector3 skyCubePosition = sphere_->GetPosition();
		skyCubePosition.y = SKYCUBE_INIT_POS;
		skyCube->SetPosition(skyCubePosition);
	}


	// 初期設定
	{
		SoundManager::Get().PlayBGM(enSoundKind_Title); // タイトルBGM再生
		inputSystem_->SetTarget(sphere_); // 操作のターゲット設定
		sphere_->SetPosition(SPHERE_INIT_POS); // タイトル時の塊の位置を設定
		g_renderingEngine->SetCascadeNearAreaRates(NEAR_AREA, MIDDLE_AREA, FAR_AREA); // カスケードシャドウのエリア率の設定
		g_sceneLight->SetDirectionLight(DIRECTION_LIGHT_NOM, DIRECTION_LIGHT_DIR, DIRECTION_LIGHT_COLOR); // 3Dオブジェクト用にディレクションライトの設定
		g_sceneLight->SetAmbinet(AMBIENT_LIGHT); // アンビエントライトの設定
		calcTime_.InitCalcTime(); // 時間を測定の初期設定
		g_camera3D->SetPosition(CAMERA_POS); // カメラの設定
		g_camera3D->SetTarget(CAMERA_TARGET_POS); // カメラのターゲットポジション
	}

	// 画像の設定
	{
		// ゲームタイトルの表示
		{
			// 画像を作成
			titleGameNameCanvas_ = new UICanvas;
			auto* icon = titleGameNameCanvas_->CreateUI<UIIcon>();
			icon->Initialize("Assets/sprite/title/titleGameName.DDS", GAME_NAME_SPRITE_WIDTH, GAME_NAME_SPRITE_HEIGHT, GAME_NAME_SPRITE_POS, Vector3::One, Quaternion::Identity);
		}

		// 「Press Button A」の表示とイージング設定
		{
			// 画像を作成
			pressButtonCanvas_ = new UICanvas;
			icon_ = pressButtonCanvas_->CreateUI<UIIcon>();
			icon_->Initialize("Assets/sprite/UI/Press_Button_A.DDS", PRESS_A_SPRITE_WIDTH, PRESS_A_SPRITE_HEIGHT, PRESS_A_SPRITE_POS, Vector3::One, Quaternion::Identity);

			// ここからイージング設定
			auto colorAnimation = std::make_unique<UIVector4Animation>();
			colorAnimation->SetParameter(PRESS_A_SPRITE_ANIMATION_COLOR_BEFORE, PRESS_A_SPRITE_ANIMATION_COLOR_AFTER, PRESS_A_ANIMATION_TIME, EasingType::EaseInOut, LoopMode::PingPong);
			colorAnimation->SetFunc([&](Vector4 v)
				{
					icon_->color_ = Vector4(v.x, v.y, v.z, v.w);
				});
			icon_->SetUIAnimation(std::move(colorAnimation));
			icon_->PlayAnimation();
		}
	}

	// ゲームループ時フェードアウトがおこなわれているのでここでフェードインする
	{
		if (!Fade::Get().IsPlay()) {
			Fade::Get().PlayFade(FadeMode::FadeIn);
		}
	}
	return true;
}

void TitleScene::Update()
{
	// 1フレームの経過時間を取得
	elapsedTime_ += g_gameTime->GetFrameDeltaTime();
	//  初期位置を0.5とし0.0～1.0を何度も増減する、
	const float lerpValue = (sinf(elapsedTime_) + LERP_CULC_ADD) * LERP_CULC_MULT;

	// アニメーション再生されてないなら
	if (!isPlayAnimation) {
		// Aボタンが押された場合s
		if (g_pad[GAME_PAD_NUM]->IsTrigger(enButtonA))
		{
			// 音を鳴らす
			SoundManager::Get().PlaySE(enSoundKind_Decision);

			// 遷移アニメーション開始
			isPlayAnimation = true;

			// ころりん動かすぞ！
			inputSystem_->SetMoveDirection(Vector3::Front);

			// フェード開始
			Fade::Get().PlayFade(FadeMode::FadeOut, NEXT_SCENE_ANIMATION_TIME, Vector3::One);

			// スタートイベントの王様の会話のためBGMを停止
			SoundManager::Get().StopBGM(true);
		}
	}
	else
	{
		// 次のシーンへ遷移OKにする
		isNextScene_ = true;
	}

	// 画像の更新
	titleGameNameCanvas_->Update();
	pressButtonCanvas_->Update();
}

void TitleScene::Render(RenderContext& rc)
{
	// 画像の描画
	titleGameNameCanvas_->Render(rc);
	pressButtonCanvas_->Render(rc);
}

bool TitleScene::RequestID(uint32_t& nextID, float& waitTime)
{
	if (isNextScene_) {
		nextID = GameScene::ID();
		waitTime = SCENE_TRANSITION_WAIT_TIME;
		return true;
	}
	return false;
}


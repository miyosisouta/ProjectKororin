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
	constexpr float NEXT_SCENE_ANIMATION_TIME = 2.0f;

}
TitleScene::TitleScene()
{
	CollisionHitManager::CreateInstance();
}

TitleScene::~TitleScene()
{
	CollisionHitManager::Delete();

	DeleteGO(sphere_);
	DeleteGO(inputSystem_);
}

bool TitleScene::Start()
{
	// 必要な機能のNewGO
	sphere_ = NewGO<Sphere>(0, "sphere");
	inputSystem_ = NewGO<TitleInputSyste>(0, "inputSystem");
	

	// ゲームループ時、スカイキューブがfalseの場合アクティブにする
	auto* skyCube = FindGO<SkyCube>("skyCube");
	if (!skyCube->IsActive()) {
		skyCube->Activate();
	}


	// 初期設定
	{
		inputSystem_->SetTarget(sphere_); // 操作のターゲット設定
		sphere_->SetPosition(Vector3(0.0f, 1.0f, -50.0f)); // タイトル時の塊の位置を設定
		g_renderingEngine->SetCascadeNearAreaRates(0.01f, 0.1f, 0.5f); // カスケードシャドウのエリア率の設定
		g_sceneLight->SetDirectionLight(0, Vector3(0.0f, -0.5f, 1.0f), Vector3(1.2f)); // 3Dオブジェクト用にディレクションライトの設定
		g_sceneLight->SetAmbinet(0.6f); // アンビエントライトの設定
		calcTime_.InitCalcTime(); // 時間を測定の初期設定
	}

	// 画像の設定
	{
		// ゲームタイトルの表示
		{
			titleGameNameCanvas_ = new UICanvas;
			auto* icon = titleGameNameCanvas_->CreateUI<UIIcon>();
			icon->Initialize("Assets/sprite/title/titleGameName.DDS", 1024.0f, 512.0f, Vector3(0.0f, 250.0f, 0.0f), Vector3::One, Quaternion::Identity);
		}

		// 「Press Button A」の表示とイージング設定
		{
			pressButtonCanvas_ = new UICanvas;
			icon_ = pressButtonCanvas_->CreateUI<UIIcon>();
			icon_->Initialize("Assets/sprite/UI/Press_Button_A.DDS", 512.0f, 256.0f, Vector3(0.0f, -250.0f, 0.0f), Vector3::One, Quaternion::Identity);
			// ここからイージング設定
			auto scaleAnimation = std::make_unique<UIVector4Animation>();
			scaleAnimation->SetParameter(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 0.1f), 3.0f, EasingType::EaseInOut, LoopMode::PingPong);
			scaleAnimation->SetFunc([&](Vector4 v)
				{
					icon_->color_ = Vector4(v.x, v.y, v.z, v.w);
				});
			icon_->SetUIAnimation(std::move(scaleAnimation));
			icon_->PlayAnimation();
		}
	}
	return true;
}

void TitleScene::Update() 
{

	// 1フレームの経過時間を取得
	elapsedTime_ += g_gameTime->GetFrameDeltaTime();
	//  初期位置を0.5とし0.0～1.0を何度も増減する、
	const float lerpValue = (sinf(elapsedTime_) + 1.0f) * 0.5f;

	// アニメーション再生されてないなら
	if (!isPlayAnimation) {
		// Aボタンが押された場合
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			// 音を鳴らす
			SoundManager::Get().PlaySE(enSoundKind_Decision);

			// 遷移アニメーション開始
			isPlayAnimation = true;

			// ころりん動かすぞ！
			inputSystem_->SetMoveDirection(Vector3::Front);

			// フェード開始
			Fade::Get().PlayFade(FadeMode::FadeOut, NEXT_SCENE_ANIMATION_TIME, Vector3::One);
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
		waitTime = 5.0f;
		return true;
	}
	return false;
}


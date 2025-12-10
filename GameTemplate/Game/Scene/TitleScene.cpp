#include "stdafx.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SphereInputSystem.h"
#include "Actor/Sphere/Sphere.h"
#include "Core/Fade.h"
#include "Collision/CollisionManager.h"
#include "Sound/SoundManager.h"

// @todo for test
#include "UI/UIBase.h"


namespace
{
	
	struct TitleGroundInformation
	{
		std::string assetPath;
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;
		

		//タイトルスプライト情報
		TitleGroundInformation(const std::string& path, const Vector3& pos, const Quaternion rot, const Vector3& scal)
			: assetPath(path)
			, position(pos)
			, scale(scal)
			, rotation(rot)
		{
		}
	};

	static const TitleGroundInformation titleModelInfoList[EnTitleGroundKind_Max] = {
		TitleGroundInformation("Assets/modelData/stage/title/titleText_P.tkm",		Vector3(-70.0f,1.0f,50.0f),	Quaternion::Identity,	Vector3(1.0f,1.0f,1.0f)),
		TitleGroundInformation("Assets/modelData/stage/title/titleText_L.tkm",		Vector3(-35.0f,1.0f,50.0f),	Quaternion::Identity,	Vector3(1.0f,1.0f,1.0f)),
		TitleGroundInformation("Assets/modelData/stage/title/titleText_A.tkm",		Vector3(0.0f,1.0f,50.0f),	Quaternion::Identity,	Vector3(1.0f,1.0f,1.0f)),
		TitleGroundInformation("Assets/modelData/stage/title/titleText_Y.tkm",		Vector3(35.0f,1.0f,50.0f),	Quaternion::Identity,	Vector3(1.0f,1.0f,1.0f)),
	};

	constexpr float NEXT_SCENE_ANIMATION_TIME = 2.0f;


	// @todo for test
	static UICanvas* canvas_ = nullptr;

}
TitleScene::TitleScene()
{
	CollisionHitManager::CreateInstance();
}

TitleScene::~TitleScene()
{
	CollisionHitManager::Delete();

	DeleteGO(skyCube_);
	DeleteGO(sphere_);
	DeleteGO(inputSystem_);
}

bool TitleScene::Start()
{
	// オブジェクトの初期化
	for (int i = 0; i < EnTitleGroundKind_Max; i++) 
	{
		const auto& titleGroundInfo = titleModelInfoList[i];
		titleGround_[i].Init(titleGroundInfo.assetPath.c_str());
		titleGround_[i].SetTRS(
			titleGroundInfo.position,
			titleGroundInfo.rotation,
			titleGroundInfo.scale
		);
	}

	// 画像の初期化
	titleGameNameSprite_.Init("Assets/Sprite/title/titleGameName.DDS", 1920, 1080);


	// 必要な機能のNewGO
	sphere_ = NewGO<Sphere>(0, "sphere");
	inputSystem_ = NewGO<TitleInputSyste>(0, "inputSystem");
	skyCube_ = NewGO<SkyCube>(0, "skyCube");
	

	// 初期設定
	inputSystem_->SetTarget(sphere_);
	sphere_->SetPosition(Vector3(0.0f,1.0f,-50.0f));

	skyCube_->SetType(enSkyCubeType_DayToon);
	skyCube_->SetScale(150.0f);
	g_renderingEngine->SetAmbientByIBLTexture(skyCube_->GetTextureFilePath(), 0.5f);
	g_renderingEngine->SetCascadeNearAreaRates(0.01f, 0.1f, 0.5f);
	
	// @todo for test
	canvas_ = new UICanvas;
	static auto* icon = canvas_->CreateUI<UIIcon>();
	icon->Initialize("Assets/sprite/title/1.DDS", 1920.0f, 1080.0f, Vector3::Zero, Vector3::One, Quaternion::Identity);
	auto* scaleAnimation = new UIVector2Animation();
	scaleAnimation->SetParameter(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f), 10.0f, EasingType::EaseInOut, LoopMode::PingPong);
	scaleAnimation->SetFunc([&](Vector2 v)
		{
			icon->m_transform.m_localScale = Vector3(v.x, v.y, 0.0f);
		});
	icon->SetUIAnimation(scaleAnimation);

	return true;
}

void TitleScene::Update() 
{
	// 1フレームの経過時間を取得
	const float deltaTime = g_gameTime->GetFrameDeltaTime();

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

	// タイトルステージの更新
	for (int i = 0; i < EnTitleGroundKind_Max; i++) {
		titleGround_[i].Update(); 
	}

	// ゲーム名画像の更新
	titleGameNameSprite_.Update(); 


	// @todo for test
	canvas_->Update();
}

void TitleScene::Render(RenderContext& rc) 
{
	if (!isNextScene_)
	{
		// ステージオブジェクト
		for (int i = 0; i < EnTitleGroundKind_Max; i++)
		{
			titleGround_[i].Draw(rc); // タイトルステージの更新
		}

		// ゲーム名
		titleGameNameSprite_.Draw(rc);
	}

	// @todo for test
	canvas_->Render(rc);
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


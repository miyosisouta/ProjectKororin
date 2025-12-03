#include "stdafx.h"
#include "Fade.h"

namespace 
{
	static const Vector3 LOADING_STATIC_SPRITE_POS = Vector3(500.0f, -380.0f, 0.0f);	// 「Loading」の座標
	static const Vector3 LOADING_MOVE_SPRITE_POS = Vector3(750.0f, -380.0f, 0.0f);		// Loading画面のアニメーションする画像の座標
	constexpr const uint16_t FADE_SPRITE_WIDTH = 1920;		// フェード用画像の幅
	constexpr const uint16_t FADE_SPRITE_HEIGHT = 1080;		// フェード用画像の高さ
	constexpr const uint8_t SPRITE_SIZE = 200;				// 画像の大きさ
	constexpr const float SPRITE_ANIMETION_TIME = 5.0f;		// 画像を動かす時間
}


Fade* Fade::instance_ = nullptr; //初期化


Fade::Fade()
{
	/** 画面全体をホワイトアウトさせる画像 **/
	fadeSprite_.Init("Assets/Sprite/Fade/TimeUp.DDS", FADE_SPRITE_WIDTH, FADE_SPRITE_HEIGHT);
	fadeSprite_.SetMulColor(fadeColorPreset::WHITE_COLOR_INVISIBLE); // 最初は透明

	/** 「Loading」と書かれた画像 **/
	loadingStaticSprite_.Init("Assets/Sprite/Fade/loadingTextSprite.DDS", SPRITE_SIZE, SPRITE_SIZE);
	loadingStaticSprite_.SetPosition(LOADING_STATIC_SPRITE_POS);
	loadingStaticSprite_.SetScale(2.0f);
	loadingStaticSprite_.SetMulColor(fadeColorPreset::WHITE_COLOR);
	
		/** Loadingアニメーション用画像 **/
	loadingMoveSprite_.Init("Assets/Sprite/Fade/loadingSphere.DDS", SPRITE_SIZE, SPRITE_SIZE);
	loadingMoveSprite_.SetPosition(LOADING_MOVE_SPRITE_POS);
	loadingMoveSprite_.SetScale(0.5f);
	loadingMoveSprite_.SetMulColor(fadeColorPreset::WHITE_COLOR);
}


Fade::~Fade()
{
}


void Fade::Update()
{
	elapsedTime_ += g_gameTime->GetFrameDeltaTime();

	// フェードを開始する場合
	if (isPlay_ || isLoading_)
	{
		UpdateAlpha();
		fadeSprite_.SetMulColor(Vector4(currentFadeColor_.x, currentFadeColor_.y, currentFadeColor_.z, fadeOutAlpha_));
		fadeSprite_.Update();
	}

	// ローディング画面を表示する場合
	if (isLoading_)
	{
		UpdateLoading();
		loadingStaticSprite_.SetMulColor(Vector4(currentLoadingColor_.x, currentLoadingColor_.y, currentLoadingColor_.z, fadeOutAlpha_));
		loadingMoveSprite_.SetMulColor(Vector4(currentLoadingColor_.x, currentLoadingColor_.y, currentLoadingColor_.z, fadeOutAlpha_));
		loadingStaticSprite_.Update();
		loadingMoveSprite_.Update();
	}
}

void Fade::Render(RenderContext& rc) 
{
	if (isPlay_ || isLoading_)
	{
		fadeSprite_.Draw(rc);
	}

	if (isLoading_) 
	{
		loadingStaticSprite_.Draw(rc);
		loadingMoveSprite_.Draw(rc);
	}
}


void Fade::PlayFade(FadeMode mode, float fadeTime, Vector3 color)
{
	mode_ = mode;			// モード設定
	if (mode_ == FadeMode::FadeOut)
	{
		SetAlpha (0.0f);
	}
	else if (mode_ == FadeMode::FadeIn) 
	{
		SetAlpha(1.0f);
	}

	fadeTime_ = fadeTime;	// フェード時間設定
	currentFadeColor_ = color;	// 色の設定
	elapsedTime_ = 0.0f;	// 経過時間の初期化

	isPlay_ = true;			// フェード開始
}


void Fade::UpdateAlpha()
{
	// α値の計算
	float fadeAlphaValue = min(elapsedTime_ / fadeTime_, 1.0f);

	// フェードインする場合
	if (mode_ == FadeMode::FadeIn) 
	{
		fadeAlphaValue = 1.0f - fadeAlphaValue;
	}

	SetAlpha(fadeAlphaValue); // α値の設定

	// α値が1.0以上になる場合、初期化(floatは完全一致は使わない)
	if (fadeAlphaValue >= 1.0f)
	{
		fadeAlphaValue = 1.0f;
	}
	else if (fadeAlphaValue <= 0.0f) 
	{
		isPlay_ = false;			// フェードを停止
		mode_ = FadeMode::None;		// モードを初期化
	}
}

void Fade::UpdateLoading()
{
	float value = elapsedTime_ * 100.0f;
	if (value > 360) {
		value = 0.0f;
	}

	Quaternion rot;
	rot.SetRotationDegZ(value * -1.0f);
	loadingMoveSprite_.SetRotation(rot);

	if (elapsedTime_ >= SPRITE_ANIMETION_TIME) 
	{
		isLoading_ = false;		// ローディングを停止
	}
}




/***********************************/


FadeObject::FadeObject()
{
	Fade::CreateInstance();
}

FadeObject::~FadeObject()
{
	Fade::DestroyInstance();
}

void FadeObject::Update() 
{
	Fade::Get().Update();
}

void FadeObject::Render(RenderContext&rc)
{
	Fade::Get().Render(rc);
}
#include "stdafx.h"
#include "SphereSizeText.h"
#include "UI/BlackOverRay.h"
#include "Actor/Sphere/Sphere.h"

namespace
{
	/*** 画像の設定 ***/

	const Vector3 TEXT_BACKRAY_POS = Vector3(-700.0f, 320.0f, 0.0f); // 黒の背景の座標
	const Vector3 TEXT_BACKRAY_SCALE = Vector3(10.0f, 10.0f, 0.0f); // 黒の背景の大きさ

	const float ALWAYS_SPEED = 100.0f; // 丸い画像の回転の基準速度
	const float OBJECTIVESIZE = 1000.0f; // 塊の大きさの指標
	const float DEFAULTSIZE = 1.0f; // 画像のデフォルトの大きさ倍率

	// 変数作成
	// SPRITE_RENDER_INFO_LIST[0] = SpriteRenderInfoType()
	// SPRITE_RENDER_INFO_LIST[1] = SpriteRenderInfoType()
	const SpriteConstans::SpriteRenderInfo SPRITE_RENDER_INFO_LIST[] =
	{
		SpriteConstans::SpriteRenderInfo("SizeOrb_Green.DDS"		, 200, Vector3(-700.0f, 320.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
		SpriteConstans::SpriteRenderInfo("SizeOrb_Blue.DDS"			, 200, Vector3(-700.0f, 320.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
		SpriteConstans::SpriteRenderInfo("SizeOrb_Pink.DDS"			, 240, Vector3(-700.0f, 320.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
		SpriteConstans::SpriteRenderInfo("indicatorSizeSprite_.DDS"	, 190, Vector3(-700.0f, 270.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
	};


	// 画像の種類
	struct SpriteRenderInfoSphereSizeType
	{
		enum Enum
		{
			SizeOrb_Green,
			SizeOrb_Blue,
			SizeOrb_Pink,
			indicatorSizeSprite
		};
	};


	/*** 文字の設定 ***/
	const int METERS_TO_CENTIMETERS = 100;

}


SphereSizeText::SphereSizeText()
{
}

SphereSizeText::~SphereSizeText()
{
}

bool SphereSizeText::Start()
{
	/*** タイマーをみやすくするための黒の背景 ***/
	RenderDarkOverlay(&textBackSprite_, TEXT_BACKRAY_POS, TEXT_BACKRAY_SCALE); // ファイルパス・初期化
	textBackSprite_.Update(); // ポジションなどの更新


	/*** 緑色の回転する玉の背景 ***/
	const auto& greenOrbInfo = SPRITE_RENDER_INFO_LIST[SpriteRenderInfoSphereSizeType::SizeOrb_Green];
	Initialize
	(
		&textGreenOrbSprite_,
		greenOrbInfo.filePath,
		greenOrbInfo.size,
		greenOrbInfo.pos,
		greenOrbInfo.scale,
		greenOrbInfo.color
	);


	/*** 青色の回転する玉の背景 ***/
	const auto& blueOrbInfo = SPRITE_RENDER_INFO_LIST[SpriteRenderInfoSphereSizeType::SizeOrb_Blue];
	Initialize
	(
		&textBlueOrbSprite_,
		blueOrbInfo.filePath,
		blueOrbInfo.size,
		blueOrbInfo.pos,
		blueOrbInfo.scale,
		blueOrbInfo.color
	);


	/*** ピンク色の回転する玉の背景 ***/
	const auto& pinkOrbInfo = SPRITE_RENDER_INFO_LIST[SpriteRenderInfoSphereSizeType::SizeOrb_Pink];
	Initialize
	(
		&textPinkOrbSprite_,
		pinkOrbInfo.filePath,
		pinkOrbInfo.size,
		pinkOrbInfo.pos,
		pinkOrbInfo.scale,
		pinkOrbInfo.color
	);


	/*** ピンク色の回転する玉の背景 ***/
	const auto& indicatorSizeInfo = SPRITE_RENDER_INFO_LIST[SpriteRenderInfoSphereSizeType::indicatorSizeSprite];
	Initialize
	(
		&indicatorSizeSprite_,
		indicatorSizeInfo.filePath,
		indicatorSizeInfo.size,
		indicatorSizeInfo.pos,
		indicatorSizeInfo.scale,
		indicatorSizeInfo.color
	);
	indicatorSizeSprite_.SetPivot(Vector2(0.5f, 1.0f));
	indicatorSizeSprite_.Update();
	return true;
}

void SphereSizeText::Update()
{
	Rotation(); // 画像の回転
	AdjustOrbScaleByRadius(); // 画像の大きさ
	ApplyScaleToUIElement(); // Sphereの現在の大きさを表示

	textGreenOrbSprite_.Update();
	textBlueOrbSprite_.Update();
	textPinkOrbSprite_.Update();
}

void SphereSizeText::Render(RenderContext& rc)
{
	textBackSprite_.Draw(rc);
	textGreenOrbSprite_.Draw(rc);
	textBlueOrbSprite_.Draw(rc);
	textPinkOrbSprite_.Draw(rc);
	indicatorSizeSprite_.Draw(rc);
	sphereSizeText_.Draw(rc);
}


void SphereSizeText::Rotation() 
{
	// 1フレームの経過時間を取得
	float orbMoveSpeed = ALWAYS_SPEED * g_gameTime->GetFrameDeltaTime();

	// 回転速度を計算
	rotationBuleSprite_.AddRotationDegZ(orbMoveSpeed);
	rotationPinkSprite_.AddRotationDegZ(orbMoveSpeed * 2.5);
	rotationGreenSprite_.AddRotationDegZ(orbMoveSpeed * (-1.2f));

	// 1フレームの回転速度をセット
	textBlueOrbSprite_.SetRotation(rotationBuleSprite_);
	textPinkOrbSprite_.SetRotation(rotationPinkSprite_);
	textGreenOrbSprite_.SetRotation(rotationGreenSprite_);
}


void SphereSizeText::AdjustOrbScaleByRadius()
{
	float progressRate = sphereRadius_ / OBJECTIVESIZE + 1.0f;
	textGreenOrbSprite_.SetScale(progressRate);
	textBlueOrbSprite_.SetScale(progressRate);
	textPinkOrbSprite_.SetScale(progressRate);
}


void SphereSizeText::ApplyScaleToUIElement()
{
	const int radiusMeters = sphereRadius_ / METERS_TO_CENTIMETERS; // メートルを算出
	const int radiusCentimeters = (int)sphereRadius_ % METERS_TO_CENTIMETERS; // センチメートルを算出

	//塊の大きさを定義・表示
	wchar_t text[256];
	swprintf_s(text, 256, L"%02dm %02dcm", radiusMeters, radiusCentimeters);
	sphereSizeText_.SetText(text);
	sphereSizeText_.SetPosition(-780.0f, 340.0f, 0.0f);
	sphereSizeText_.SetScale(1.0f);
	sphereSizeText_.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}


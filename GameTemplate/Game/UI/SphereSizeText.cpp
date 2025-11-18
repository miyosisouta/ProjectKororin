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
	const float OBJECTIVESIZE = 500.0f; // 塊の大きさの指標
	const float DEFAULTSIZE = 1.0f; // 画像のデフォルトの大きさ倍率
	const float SPHERESIZETEXT_DRAWLIMMIT = 300.0f; // 塊の目標サイズのテキスト表示を制限する

	// 変数作成
	// SPRITE_RENDER_INFO_LIST[0] = SpriteRenderInfoType()
	// SPRITE_RENDER_INFO_LIST[1] = SpriteRenderInfoType()
	const SpriteConstans::SpriteRenderInfo SPRITE_RENDER_INFO_LIST[] =
	{
		SpriteConstans::SpriteRenderInfo("SizeOrb_Green.DDS"		, 200, Vector3(-700.0f, 320.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
		SpriteConstans::SpriteRenderInfo("SizeOrb_Blue.DDS"			, 200, Vector3(-700.0f, 320.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
		SpriteConstans::SpriteRenderInfo("SizeOrb_Pink.DDS"			, 240, Vector3(-700.0f, 320.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
		SpriteConstans::SpriteRenderInfo("indicatorSizeSprite_.DDS"	, 190, Vector3(-700.0f, 270.0f, 0.0f), Vector3(1.0f, 0.9f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
		SpriteConstans::SpriteRenderInfo("GoalSize_OverLay.DDS"		, 190, Vector3(-525.0f, 415.0f, 0.0f), Vector3(0.6f, 0.5f, 0.5f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
		SpriteConstans::SpriteRenderInfo("GoalSprite_ColorCorn.DDS"	, 190, Vector3(-565.0f, 415.0f, 0.0f), Vector3(0.25f, 0.25f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
	};


	// 画像の種類
	struct SpriteRenderInfoSphereSizeType
	{
		enum Enum
		{
			SizeOrb_Green,
			SizeOrb_Blue,
			SizeOrb_Pink,
			indicatorSizeSprite,
			GoalSizeOverRaySprite,
			GoalSizeColorCornSprite,
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
	/*** UIをみやすくするための黒の背景 ***/
	RenderDarkOverlay(&textBackSprite_, TEXT_BACKRAY_POS, TEXT_BACKRAY_SCALE); // ファイルパス・初期化
	textBackSprite_.Update(); // ポジションなどの更新

	/*** 塊の目標サイズのデコレーションの背景 ***/
	const auto& GoalSizeColorCornInfo = SPRITE_RENDER_INFO_LIST[SpriteRenderInfoSphereSizeType::GoalSizeColorCornSprite];
	Initialize
	(
		&GoalSizeColorCornSprite_,
		GoalSizeColorCornInfo.filePath,
		GoalSizeColorCornInfo.size,
		GoalSizeColorCornInfo.pos,
		GoalSizeColorCornInfo.scale,
		GoalSizeColorCornInfo.color
	);


	/*** 目標サイズを見やすくするための背景 ***/
	const auto& GoalSizeOverLayInfo = SPRITE_RENDER_INFO_LIST[SpriteRenderInfoSphereSizeType::GoalSizeOverRaySprite];
	Initialize
	(
		&GoalSizeOverRaySprite_,
		GoalSizeOverLayInfo.filePath,
		GoalSizeOverLayInfo.size,
		GoalSizeOverLayInfo.pos,
		GoalSizeOverLayInfo.scale,
		GoalSizeOverLayInfo.color
	);


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
	SphereGoalSizeText();
	textGreenOrbSprite_.Update();
	textBlueOrbSprite_.Update();
	textPinkOrbSprite_.Update();
}

void SphereSizeText::Render(RenderContext& rc)
{
	textBackSprite_.Draw(rc);

	if (sphereRadius_ <= SPHERESIZETEXT_DRAWLIMMIT)
	{
		GoalSizeOverRaySprite_.Draw(rc);
		GoalSizeColorCornSprite_.Draw(rc);
		sphereGoalSizeText_.Draw(rc);
	}
	
	textGreenOrbSprite_.Draw(rc);
	textBlueOrbSprite_.Draw(rc);
	textPinkOrbSprite_.Draw(rc);
	indicatorSizeSprite_.Draw(rc);
	sphereCurrentSizeText_.Draw(rc);
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
	wchar_t currentSize[256];
	swprintf_s(currentSize, 256, L"%02dm %02dcm", radiusMeters, radiusCentimeters);
	sphereCurrentSizeText_.SetText(currentSize);
	sphereCurrentSizeText_.SetPosition(-780.0f, 340.0f, 0.0f);
	sphereCurrentSizeText_.SetScale(1.0f);
	sphereCurrentSizeText_.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void SphereSizeText::SphereGoalSizeText()
{

	//塊の大きさを定義・表示
	wchar_t goalSize[256];
	swprintf_s(goalSize, 256, L"3m");
	sphereGoalSizeText_.SetText(goalSize);
	sphereGoalSizeText_.SetPosition(-545.0f, 430.0f, 0.0f);
	sphereGoalSizeText_.SetScale(0.9f);
	sphereGoalSizeText_.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}


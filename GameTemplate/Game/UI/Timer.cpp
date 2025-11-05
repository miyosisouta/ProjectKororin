#include "stdafx.h"
#include "Timer.h"
#include "UI/BlackOverRay.h"


namespace
{
	const float GAMEPLAY_TIMELIMIT = 120.0f;
	const Vector2 TIMELINESPRITE_PIVOT = Vector2(0.5f, 0.25f); // タイマーの針の基点の座標
	const Vector3 TIME_BACKRAY_POS = Vector3(750.0f, 400.0f, 0.0f); // 黒の背景の座標
	const Vector3 TIME_BACKRAY_SCALE = Vector3(3.5f, 3.5f, 1.0f); // 黒の背景の座標


	// 変数作成
	// SPRITE_RENDER_INFO_LIST[0] = SpriteRenderInfoType()
	// SPRITE_RENDER_INFO_LIST[1] = SpriteRenderInfoType()
	const SpriteConstans::SpriteRenderInfo SPRITE_RENDER_INFO_LIST[] =
	{
		SpriteConstans::SpriteRenderInfo("TimeGauge.DDS",		256, Vector3(750.0f, 400.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
		SpriteConstans::SpriteRenderInfo("TimeLimitLine.DDS",	128, Vector3(750.0f, 400.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
	};


	// 画像の種類
	struct SpriteRenderInfoTimerType
	{
		enum Enum
		{
			TimeGauge,
			TimeLimitLine,
		};
	};
}


Timer::Timer()
{
}

Timer::~Timer()
{
}

bool Timer::Start()
{
	/*** タイマーをみやすくするための黒の背景 ***/
	RenderDarkOverlay(&clockBackSprite_, TIME_BACKRAY_POS, TIME_BACKRAY_SCALE); // ファイルパス・初期化
	clockBackSprite_.Update(); // ポジションなどの更新


	/***  タイマーのスプライト ***/
	const auto& timeGaugeInfo = SPRITE_RENDER_INFO_LIST[SpriteRenderInfoTimerType::TimeGauge];
	Initialize
	(
		&timeGaugeSprite_,
		timeGaugeInfo.filePath,
		timeGaugeInfo.size,
		timeGaugeInfo.pos,
		timeGaugeInfo.scale,
		timeGaugeInfo.color
	);


	/*** タイマーの針 ***/
	const auto& timeLimitLine = SPRITE_RENDER_INFO_LIST[SpriteRenderInfoTimerType::TimeLimitLine];
	Initialize
	(
		&timeLineSprite_,
		timeLimitLine.filePath,
		timeLimitLine.size,
		timeLimitLine.pos, 
		timeLimitLine.scale,
		timeLimitLine.color
	);

	timeLineSprite_.SetPivot(TIMELINESPRITE_PIVOT);
	timeLineSprite_.Update();

	gameTimer_ = GAMEPLAY_TIMELIMIT;
	return true;
}

void Timer::Update()
{
	UpdateLineValue();
	timeLineSprite_.Update();
}

void Timer::Render(RenderContext& rc)
{
	clockBackSprite_.Draw(rc);
	timeGaugeSprite_.Draw(rc);
	timeLineSprite_.Draw(rc);
}


void Timer::UpdateLineValue()
{
	gameTimer_ -= g_gameTime->GetFrameDeltaTime();
	float progressRate =gameTimer_ / GAMEPLAY_TIMELIMIT; // 1フレームの回転比率を計算
	float angle = progressRate * 360.0f; // 回転角度
	rotation_.SetRotationDegZ(angle); // z座標を軸にQuaternionを制作
	timeLineSprite_.SetRotation(rotation_); // 回す
}

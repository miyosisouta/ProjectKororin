#include "stdafx.h"
#include "Timer.h"
#include "UI/BlackOverRay.h"


namespace
{
	const float GAMEPLAY_TIMELIMIT = 120.0f;
	const Vector2 TIMELINESPRITE_PIVOT = Vector2(0.5f, 0.25f); // タイマーの針の基点の座標
	const Vector3 TIME_BACKRAY = Vector3(750.0f, 400.0f, 0.0f); // 黒の背景の座標

	// スプライトに必要な情報
	struct SpriteRenderInfo 
	{
		std::string filePath; // ファイルパス
		int size; // ピクセルサイズ
		Vector3 pos; // 座標
		Vector3 scale; // 画像サイズの倍率
		Vector4 color; // 乗算カラー


		// スプライトに必要な情報を設定
		SpriteRenderInfo(const std::string path, const int s, const Vector3& p, const Vector3& scal, const Vector4& c)
			: filePath(path)
			, size(s)
			, pos(p)
			, scale(scal)
			, color(c)
		{
		}
	};


	// 変数作成
	// SPRITE_RENDER_INFO_LIST[0] = SpriteRenderInfoType()
	// SPRITE_RENDER_INFO_LIST[1] = SpriteRenderInfoType()
	SpriteRenderInfo SPRITE_RENDER_INFO_LIST[] =
	{
		SpriteRenderInfo("TimeGauge.DDS", 256, Vector3(750.0f, 400.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
		SpriteRenderInfo("TimeLimitLine.DDS", 128, Vector3(750.0f, 400.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
	};


	// 画像の種類
	struct SpriteRenderInfoType
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
	RenderDarkOverlay(&clockBackRender_, TIME_BACKRAY); // ファイルパス・初期化
	clockBackRender_.Update(); // ポジションなどの更新


	/***  タイマーのスプライト ***/
	const auto& timeGaugeInfo = SPRITE_RENDER_INFO_LIST[SpriteRenderInfoType::TimeGauge];
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
	const auto& timeLimitLine = SPRITE_RENDER_INFO_LIST[SpriteRenderInfoType::TimeLimitLine];
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
	clockBackRender_.Draw(rc);
	timeGaugeSprite_.Draw(rc);
	timeLineSprite_.Draw(rc);
}


void Timer::UpdateLineValue()
{
	gameTimer_ -= g_gameTime->GetFrameDeltaTime();
	float progressRate =gameTimer_ / GAMEPLAY_TIMELIMIT; // 回転比率を計算
	float angle = progressRate * 360.0f; // 回転角度
	rotation_.SetRotationDegZ(angle);
	timeLineSprite_.SetRotation(rotation_); // 回す
}

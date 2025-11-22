#include "stdafx.h"
#include "Timer.h"
#include "UI/BlackOverRay.h"
#include "Core/Fade.h"
#include "Core/GameTimer.h"


namespace
{

	const Vector2 TIMELINESPRITE_PIVOT = Vector2(0.5f, 0.25f); // タイマーの針の基点の座標
	const Vector3 TIME_BACKRAY_POS = Vector3(750.0f, 400.0f, 0.0f); // 黒の背景の座標
	const Vector3 TIME_BACKRAY_SCALE = Vector3(3.5f, 3.5f, 1.0f); // 黒の背景の大きさ
	const float ONE_LAP = 360.0f;


	// フェードの境界時間を定数として定義
	const float FADE_START_TIME = 20.0f; // フェード開始 (α = 0.0f)
	const float FADE_MID_TIME = 10.0f; // 中間点 (α = 0.3f)
	const float FADE_END_TIME = 0.0f;  // フェード終了 (α = 1.0f)

	const float STAGE_DURATION = 10.0f; // 各ステージの長さ (10秒)
	const float ALPHA_MAX_FIRST_HALF = 0.3f;  // ステージ1の最大アルファ値
	const float ALPHA_RANGE_SECOND_HALF = 0.7f;  // ステージ2のアルファ値の増加幅 (1.0f - 0.3f)


	// 変数作成
	// SPRITE_RENDER_INFO_LIST[0] = SpriteRenderInfoType()
	// SPRITE_RENDER_INFO_LIST[1] = SpriteRenderInfoType()
	const SpriteConstans::SpriteRenderInfo SPRITE_RENDER_INFO_LIST[] =
	{
		SpriteConstans::SpriteRenderInfo("TimeGauge.DDS",				256,	Vector3(750.0f, 400.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
		SpriteConstans::SpriteRenderInfo("TimeLimitLine.DDS",			128,	Vector3(750.0f, 400.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),		
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

	return true;
}

void Timer::Update()
{
	UpdateLineValue();
	CalculateFadeAlphaByTime();
	timeLineSprite_.Update();
	Fade::Get().Update();
}

void Timer::Render(RenderContext& rc)
{
	clockBackSprite_.Draw(rc);
	timeGaugeSprite_.Draw(rc);
	timeLineSprite_.Draw(rc);
}


void Timer::UpdateLineValue()
{
	float progressRate =gameTimer_ / gameTimeLimit; // 1フレームの回転比率を計算
	float angle = progressRate * ONE_LAP; // 回転角度
	rotation_.SetRotationDegZ(angle); // z座標を軸にQuaternionを制作
	timeLineSprite_.SetRotation(rotation_); // 回す
}

void Timer::CalculateFadeAlphaByTime()
{
	if (GameTimer::Get().GetRemainingTime() > 20.0f) { return; } // 残り時間が20秒切っていない場合は処理を返す

	float currentTime = GameTimer::Get().GetRemainingTime(); // 現在の残り時間を取得
	

	// 20秒から10秒までの処理
	if (currentTime >= 10.0f) 
	{
		float calcAlpha = (FADE_START_TIME - currentTime) / STAGE_DURATION; // 経過時間による比率の計算(最大1.0)
		float fadeAlpha = calcAlpha * ALPHA_MAX_FIRST_HALF; //最大アルファ値(0.3f)を乗算し、最終的なα値を計算
		whiteOutAlpha = fadeAlpha;
	}

	// 10秒からタイムアップまで
	else if (currentTime <= 10.0f) 
	{
		float calcAlpha = (FADE_MID_TIME - currentTime) / STAGE_DURATION; // 経過時間による比率の計算(最大1.0)
		float fadeAlpha = ALPHA_MAX_FIRST_HALF + calcAlpha * ALPHA_RANGE_SECOND_HALF;//前半のMax増加幅と後半の増加幅(0.7f)を乗算した値を加算し、最終的なα値を計算
		whiteOutAlpha = fadeAlpha;
	}
	Fade::Get().SetAlpha(whiteOutAlpha); // データを送る
}

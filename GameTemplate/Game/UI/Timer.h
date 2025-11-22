#pragma once
#include "UI/BlackOverRay.h"

class Timer : public BlackOverRay
{
public:
	Timer();
	~Timer();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	/**
	 * UIの線が制限時間をもとに回るよう更新
	 */
	void UpdateLineValue();

	/**
	 * 残り時間によってα値を計算、Fadeクラスにて設定
	 */
	void CalculateFadeAlphaByTime();


public:
	void SetTimer(const float timer) { gameTimer_ = timer; }
	void SetGameTimerLimit(const float limitTimer) { gameTimeLimit = limitTimer; };


private:
	SpriteRender clockBackSprite_; //!< タイマーをみやすくするための黒の背景
	SpriteRender timeGaugeSprite_; //!< タイマーの画像
	SpriteRender timeLineSprite_; //!< タイマーの針
	Quaternion rotation_ = Quaternion::Identity; 

	float gameTimer_ = 0.0f;
	float gameTimeLimit = 0.0f;
	float whiteOutAlpha = 0.0f;
};


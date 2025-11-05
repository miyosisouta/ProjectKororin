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
	 * @brief UIの線が制限時間をもとに回るよう更新
	 */
	void UpdateLineValue();


private:
	SpriteRender clockBackSprite_; //!< タイマーをみやすくするための黒の背景
	SpriteRender timeGaugeSprite_; //!< タイマーのスプライト
	SpriteRender timeLineSprite_; //!< タイマーの針
	Quaternion rotation_ = Quaternion::Identity; 

	float gameTimer_ = 0.0f;
};


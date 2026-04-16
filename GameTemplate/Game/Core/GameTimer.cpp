#include "stdafx.h"
#include "GameTimer.h"

namespace
{
	constexpr float INIT_TIME = 0.0f;
}

GameTimer* GameTimer::instance_ = nullptr; //èâä˙âª


void GameTimer::Update()
{
	if (remainingTime_ == INIT_TIME) { return; } // écÇËéûä‘Ç™0ïbÇ…Ç»Ç¡ÇΩèÍçáåvéZÇµÇ»Ç¢

	// éûä‘ÇåvéZ
	const float deltaTime = g_gameTime->GetFrameDeltaTime();
	remainingTime_ -= deltaTime;

	// 0ÇÊÇËè¨Ç≥Ç≠Ç»ÇÁÇ»Ç¢ÇÊÇ§Ç…
	if (remainingTime_ < INIT_TIME) {
		remainingTime_ = INIT_TIME;
	}
}
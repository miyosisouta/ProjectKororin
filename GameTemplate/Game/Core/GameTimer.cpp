#include "stdafx.h"
#include "GameTimer.h"

namespace 
{
}


GameTimer* GameTimer::instance_ = nullptr; //èâä˙âª


GameTimer::GameTimer()
{

}


GameTimer::~GameTimer()
{
}


void GameTimer::Update()
{
	if (remainingTime_ == 0.0f) { return; }// écÇËéûä‘Ç™0ïbÇ…Ç»Ç¡ÇΩèÍçáåvéZÇµÇ»Ç¢

	const float deltaTime = g_gameTime->GetFrameDeltaTime();
	remainingTime_ -= deltaTime;
	// 0ÇÊÇËè¨Ç≥Ç≠Ç»ÇÁÇ»Ç¢ÇÊÇ§Ç…
	if (remainingTime_ < 0.0f) {
		remainingTime_ = 0.0f;
	}
}
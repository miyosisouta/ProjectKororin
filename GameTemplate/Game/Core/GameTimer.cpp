#include "stdafx.h"
#include "GameTimer.h"

GameTimer* GameTimer::instance_ = nullptr; //‰Šú‰»


void GameTimer::Update()
{
	if (remainingTime_ == 0.0f) { return; } // c‚èŠÔ‚ª0•b‚É‚È‚Á‚½ê‡ŒvZ‚µ‚È‚¢

	const float deltaTime = g_gameTime->GetFrameDeltaTime();// Œo‰ßŠÔ
	remainingTime_ -= deltaTime;
	// 0‚æ‚è¬‚³‚­‚È‚ç‚È‚¢‚æ‚¤‚É
	if (remainingTime_ < 0.0f) {
		remainingTime_ = 0.0f;
	}
}
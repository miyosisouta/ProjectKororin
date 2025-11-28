#include "stdafx.h"
#include "ResultScene.h"
#include "Core/Fade.h"
#include "TitleScene.h"


ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

bool ResultScene::Start()
{
	//Fade::Get().SetAlpha(0.0f);
	//resultSprite_.Init("Assets/Sprite/Result/Result.DDS", MAX_SPRITE_WIDTH, MAX_SPRITE_HIGHT);
	return true;
}

void ResultScene::Update()
{
	//resultSprite_.Update();
}

void ResultScene::Render(RenderContext& rc)
{
	//resultSprite_.Draw(rc);
}

bool ResultScene::RequestID(uint32_t& id)
{
	if (isNextScene_) {
		id = TitleScene::ID();
		return true;
	}
	return false;
}

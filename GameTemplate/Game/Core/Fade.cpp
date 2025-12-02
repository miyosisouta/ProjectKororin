#include "stdafx.h"
#include "Fade.h"

namespace 
{
}


Fade* Fade::instance_ = nullptr; //初期化


Fade::Fade()
{
	/** 画面全体をホワイトアウトさせる画像 **/
	fadeOutSprite_.Init("Assets/Sprite/Game/TimeUp.DDS", 1920, 1080);
	fadeOutSprite_.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f)); // 最初は透明
}


Fade::~Fade()
{
}


void Fade::Update()
{
	fadeOutSprite_.SetMulColor(Vector4(fadeColor_, fadeColor_, fadeColor_, fadeOutAlpha_));

	fadeOutSprite_.Update();
}


void Fade::Render(RenderContext& rc) 
{
	fadeOutSprite_.Draw(rc);
}


/***********************************/


FadeObject::FadeObject()
{
	Fade::CreateInstance();
}

FadeObject::~FadeObject()
{
	Fade::DestroyInstance();
}

void FadeObject::Update() 
{
	Fade::Get().Update();
}

void FadeObject::Render(RenderContext&rc)
{
	Fade::Get().Render(rc);
}

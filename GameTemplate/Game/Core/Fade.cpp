#include "stdafx.h"
#include "Fade.h"

namespace 
{
}


Fade* Fade::instance_ = nullptr; //初期化


Fade::Fade()
{
	/** 画面全体をホワイトアウトさせる画像 **/
	whiteOutSprite_.Init("Assets/Sprite/UI/TimeUp.DDS", 1920, 1080);
	whiteOutSprite_.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f)); // 最初は透明
}


Fade::~Fade()
{
}


void Fade::Update()
{
	whiteOutSprite_.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, whiteOutAlpha_));
	whiteOutSprite_.Update();
}


void Fade::Render(RenderContext& rc) 
{
	whiteOutSprite_.Draw(rc);
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

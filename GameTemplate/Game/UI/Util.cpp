#include "stdafx.h"
#include "Util.h"



void UIUtil::SetAButtonUI(FontRender* font, SpriteRender* sprite, Vector3 spritePos, Vector3 fontPos, Vector3 spriteScal, float fontScal)
{
	// テキストの設定
	wchar_t text[256];
	swprintf_s(text, 256, L"Press");
	font->SetText(text);
	font->SetPSC(
		fontPos,
		fontScal,
		Vector4::White
	);

	// Aボタンの画像
	sprite->Init("Assets/sprite/UI/Button_A.DDS", 256, 256);
	sprite->SetPSM(
		spritePos,
		spriteScal,
		Vector4::White
	);

}


void UIUtil::SetText(FontRender* fontRender, const std::function<void(wchar_t*)>& func)
{
	wchar_t text[256];
	func(text);
	fontRender->SetText(text);
}

#include "stdafx.h"
#include "Util.h"


void UIUtil::SetAButtonUI(SpriteRender* spritePath, FontRender* font, Vector3 spritePos, Vector3 fontPos)
{
	// Aボタンの画像
	spritePath->Init("Assets/sprite/UI/Button_A.DDS", 256, 256);
	spritePath->SetPSM(
		spritePos,
		Vector3::One,
		Vector4::White
	);

	// テキストの設定
	wchar_t text[256];
	swprintf_s(text, 256, L"をおしてね");
	font->SetText(text);
	font->SetPSC(
		fontPos,
		2.0f,
		Vector4::White
	);
}


void UIUtil::SetText(FontRender* fontRender, const std::function<void(wchar_t*)>& func)
{
	wchar_t text[256];
	func(text);
	fontRender->SetText(text);
}
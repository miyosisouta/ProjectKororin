#include "stdafx.h"
#include "Util.h"

namespace
{
	constexpr uint16_t TEXT_COUNT_MAX = 256;
	constexpr float BUTTON_SPRITE_SIZE = 256.0f;
}

void UIUtil::SetAButtonUI(FontRender* font, SpriteRender* sprite, Vector3 spritePos, Vector3 fontPos, Vector3 spriteScal, float fontScal)
{
	// テキストの設定
	wchar_t text[TEXT_COUNT_MAX];
	swprintf_s(text, TEXT_COUNT_MAX, L"Press");
	font->SetText(text);
	font->SetPSC(
		fontPos,
		fontScal,
		Vector4::White
	);

	// Aボタンの画像
	sprite->Init("Assets/sprite/UI/Button_A.DDS", BUTTON_SPRITE_SIZE, BUTTON_SPRITE_SIZE);
	sprite->SetPSM(
		spritePos,
		spriteScal,
		Vector4::White
	);

}

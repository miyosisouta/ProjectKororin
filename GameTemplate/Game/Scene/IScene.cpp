#include "stdafx.h"
#include "IScene.h"

namespace
{
	constexpr float BUTTON_SPRITE_SIZE = 256.0f;
	constexpr uint16_t TEXT_MAX_LETTER = 256;
	constexpr float TEXT_SCALE = 1.0f;
}


void IScene::ButtonUI(SpriteRender& spritePath, FontRender& font, Vector3 spritePos, Vector3 fontPos)
{
	// Aボタンの画像
	spritePath.Init("Aseets/Sprite/UI/Button.DDS", BUTTON_SPRITE_SIZE, BUTTON_SPRITE_SIZE);
	spritePath.SetPSM(
		spritePos,
		Vector3::One,
		Vector4::White
	);

	// テキストの設定
	wchar_t text[TEXT_MAX_LETTER];
	swprintf_s(text, TEXT_MAX_LETTER, L"をおしてね");
	font.SetText(text);
	font.SetPSC(
		fontPos,
		TEXT_SCALE,
		Vector4::White
	);
}

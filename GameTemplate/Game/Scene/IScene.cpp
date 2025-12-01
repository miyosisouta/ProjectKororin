#include "stdafx.h"
#include "IScene.h"

void IScene::ButtonUI(SpriteRender& spritePath, FontRender& font, Vector3 spritePos, Vector3 fontPos)
{
	// Aボタンの画像
	spritePath.Init("Aseets/Sprite/UI/Button.DDS", 256, 256);
	spritePath.SetPSM(
		spritePos,
		Vector3::One,
		Vector4::White
	);

	// テキストの設定
	wchar_t text[256];
	swprintf_s(text, 256, L"をおしてね");
	font.SetText(text);
	font.SetPSC(
		fontPos,
		1.0f,
		Vector4::White
	);
}

#pragma once


class UIUtil
{
public:
	/* 「Aボタンおしてね」を表示する設定 */
	static void SetAButtonUI(FontRender* font, SpriteRender* sprite, Vector3 spritePos, Vector3 fontPos, Vector3 spriteScal = 1.0f, float fontScal = 1.0f);

	/* テキストの設定 */
	static void SetText(nsK2Engine::FontRender* fontRender, const std::function<void(wchar_t*)>& func);
};


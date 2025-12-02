#pragma once


class UIUtil
{
public:
	/* 「Aボタンおしてね」を表示する設定 */
	static void SetAButtonUI(SpriteRender* spritePath, FontRender* font, Vector3 spritePos, Vector3 fontPos);

	/* テキストの設定 */
	/* function :  */
	static void SetText(FontRender* fontRender, const std::function<void(wchar_t*)>& func);
};


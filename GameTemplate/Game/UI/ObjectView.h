#pragma once
#include "UI/BlackOverRay.h"

class ObjectView : public BlackOverRay
{
public:
	/* デストラクタ */
	~ObjectView();

	/* スタート処理 */
	bool Start() override;
	/* 描画処理 */
	void Render(RenderContext& rc) override;


private:
	SpriteRender AttachObjectBackSprite;	//!< 吸着可能オブジェクトの背景画像 
	SpriteRender PlaceHolderSprite;
};


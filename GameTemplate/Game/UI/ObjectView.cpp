#include "stdafx.h"
#include "ObjectView.h"
#include "UI/BlackOverRay.h"

namespace
{
	const Vector3 ATTACHOBJECT_BACKRAY_POS = Vector3(-700.0f, -320.0f, 0.0f);
	const Vector3 ATTACHOBJECT_BACKRAY_SCALE = Vector3(5.0f, 5.0f, 1.0f);

	// 変数作成
	// SPRITE_RENDER_INFO_LIST[0] = SpriteRenderInfoType()
	// SPRITE_RENDER_INFO_LIST[1] = SpriteRenderInfoType()
	const SpriteConstans::SpriteRenderInfo SPRITE_RENDER_INFO_LIST[] =
	{
		SpriteConstans::SpriteRenderInfo("AttachObjectView.DDS",256, Vector3(-700.0f, -320.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
	};


	// 画像の種類
	struct SpriteRenderInfoAttachObjectType
	{
		enum Enum
		{
			Nom,
		};
	};
}


ObjectView::~ObjectView()
{
}

bool ObjectView::Start()
{
	/** 吸着したオブジェクトが見えやすいようにするための画像 **/
	RenderDarkOverlay(&AttachObjectBackSprite, ATTACHOBJECT_BACKRAY_POS, ATTACHOBJECT_BACKRAY_SCALE); // ファイルパス・初期化
	AttachObjectBackSprite.Update(); // ポジションなどの更新


	/*** 塊に引っ付いたオブジェクトの画像 ***/
	const auto& attachObjectInfo = SPRITE_RENDER_INFO_LIST[SpriteRenderInfoAttachObjectType::Nom];
	Initialize
	(
		&PlaceHolderSprite,
		attachObjectInfo.filePath,
		attachObjectInfo.size,
		attachObjectInfo.pos,
		attachObjectInfo.scale,
		attachObjectInfo.color
	);
	return true;
}

void ObjectView::Update()
{

}

void ObjectView::Render(RenderContext& rc)
{
	AttachObjectBackSprite.Draw(rc);
	PlaceHolderSprite.Draw(rc);
}



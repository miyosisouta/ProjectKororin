#include "stdafx.h"
#include "ObjectView.h"
#include "UI/BlackOverRay.h"

namespace
{
	const Vector3 ATTACHOBJECT_BACKRAY_POS = Vector3(-700.0f, -320.0f, 0.0f);
	const Vector3 ATTACHOBJECT_BACKRAY_SCALE = Vector3(5.0f, 5.0f, 1.0f);
}


ObjectView::~ObjectView()
{
}

bool ObjectView::Start()
{
	/** 吸着したオブジェクトが見えやすいようにするための画像 **/
	RenderDarkOverlay(&AttachObjectBackSprite, ATTACHOBJECT_BACKRAY_POS, ATTACHOBJECT_BACKRAY_SCALE); // ファイルパス・初期化
	AttachObjectBackSprite.Update(); // ポジションなどの更新

	return true;
}

void ObjectView::Render(RenderContext& rc)
{
	AttachObjectBackSprite.Draw(rc);
}



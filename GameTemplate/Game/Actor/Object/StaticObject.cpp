#include "stdafx.h"
#include "StaticObject.h"

bool StaticObject::Start()
{
	// 親クラスの初期化処理
	StageObjectBase::Start();
	return true;
}

void StaticObject::Update()
{
	// モデルの更新
	modelRender_.Update();
}

void StaticObject::Render(RenderContext& rc)
{
	// 描画可能なら描画
	if (isVisible_) {
		modelRender_.Draw(rc);
	}
}

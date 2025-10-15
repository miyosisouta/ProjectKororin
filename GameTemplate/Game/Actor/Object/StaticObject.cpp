#include "stdafx.h"
#include "StaticObject.h"

bool StaticObject::Start()
{
	StageObjectBase::Start();
	return true;
}

void StaticObject::Update()
{
	modelRender_.Update();
}

void StaticObject::Render(RenderContext& rc)
{
	modelRender_.Draw(rc);
}

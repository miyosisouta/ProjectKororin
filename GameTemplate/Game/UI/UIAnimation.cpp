/**
 * SpriteAnimation.cpp
 * SpriteRenderをアニメーションさせる機能群
 */
#include "stdafx.h"
#include "UIAnimation.h"
#include "UIBase.h"


UIColorAnimation::UIColorAnimation()
{
	SetFunc([&](Vector4 v)
		{
			m_ui->color_ = v;
		});
}
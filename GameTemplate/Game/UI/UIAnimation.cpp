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

UIScaleAnimation::UIScaleAnimation()
{
	SetFunc([&](Vector3 s)
		{
			m_ui->m_transform.m_localScale = s;
			m_ui->m_transform.UpdateTransform();
		});
}

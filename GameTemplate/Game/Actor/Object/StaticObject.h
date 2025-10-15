/**
 * StaticObject.h
 * �z�����Ȃ��I�u�W�F�N�g���Ǘ�����N���X
 */

#pragma once
#include "StageObjectBase.h"


class StaticObject : public StageObjectBase
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	StaticObject() {};
	/**
	 * @brief �f�X�g���N�^
	 */
	~StaticObject() {};


	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;
};
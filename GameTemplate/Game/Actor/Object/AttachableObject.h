/**
 * AttachableObject.h
 * ��ɋz���\�ȃI�u�W�F�N�g���Ǘ�����N���X
 */

#pragma once
#include "StageObjectBase.h"


class AttachableObject : public StageObjectBase
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	AttachableObject() {};
	/**
	 * @brief �f�X�g���N�^
	 */
	~AttachableObject();

	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

public:
	void DeletePhysicsStatics();
};


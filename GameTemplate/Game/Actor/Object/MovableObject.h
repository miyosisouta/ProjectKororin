/**
 * MovableObject.h
 * �����I�u�W�F�N�g���Ǘ�����N���X
 */
#pragma once
#include "StageObjectBase.h"


class MovableObject : public StageObjectBase
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	MovableObject() {};
	/**
	 * @brief �f�X�g���N�^
	 */
	~MovableObject() {};


	bool Start() override;
};
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
	inline int GetObjectSize() const
	{
		return objectSize_;
	}

	inline int GetObjectValue() const
	{
		return attachableValue_;
	}

	inline int GetGrowAmount() const
	{
		return grouthAmount_;
	}

	inline void SetPlayedSE(bool isPlayedSE)
	{
		isPlayedSE_ = isPlayedSE;
	}

	inline bool IsPlayedSE() const
	{
		return isPlayedSE_;
	}


public:
	/**
	 * @brief �����蔻��̔j����nullptr
	 */
	void DeletePhysicsStatics();


private:
	bool isPlayedSE_ = false;
};


/**
 * AttachableObject.h
 * 塊に吸着可能なオブジェクトを管理するクラス
 */

#pragma once
#include "StageObjectBase.h"


class AttachableObject : public StageObjectBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	AttachableObject() {};
	/**
	 * @brief デストラクタ
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
	 * @brief 当たり判定の破棄とnullptr
	 */
	void DeletePhysicsStatics();


private:
	bool isPlayedSE_ = false;
};


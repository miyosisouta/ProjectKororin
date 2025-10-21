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
	inline const int GetObjectSize() 
	{
		return objectSize_;
	}

	inline const int GetObjectValue() 
	{
		return attachableValue_;
	}

	inline const int GetGrowAmount()
	{
		return grouthAmount_;
	}


public:
	/**
	 * @brief 当たり判定の破棄とnullptr
	 */
	void DeletePhysicsStatics();
};


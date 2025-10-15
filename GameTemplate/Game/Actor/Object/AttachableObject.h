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
	void DeletePhysicsStatics();
};


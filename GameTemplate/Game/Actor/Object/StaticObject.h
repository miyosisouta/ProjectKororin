/**
 * StaticObject.h
 * 吸着しないオブジェクトを管理するクラス
 */

#pragma once
#include "StageObjectBase.h"


class StaticObject : public StageObjectBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	StaticObject() {};
	/**
	 * @brief デストラクタ
	 */
	~StaticObject() {};


	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;
};
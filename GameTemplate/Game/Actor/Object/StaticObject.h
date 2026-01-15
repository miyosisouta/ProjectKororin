/**
 * StaticObject.h
 * 吸着しないオブジェクトを管理するクラス
 */

#pragma once
#include "StageObjectBase.h"


class StaticObject : public StageObjectBase
{
public:
	/* コンストラクタ */
	StaticObject() {};
	/* デストラクタ */
	~StaticObject() {};

	/* スタート処理 */
	bool Start()override;
	/* 更新処理 */
	void Update()override;
	/* 描画処理 */
	void Render(RenderContext& rc)override;
};
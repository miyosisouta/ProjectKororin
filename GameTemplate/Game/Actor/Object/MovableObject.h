/**
 * MovableObject.h
 * 動くオブジェクトを管理するクラス
 */
#pragma once
#include "StageObjectBase.h"


class MovableObject : public StageObjectBase
{
public:
	/* コンストラクタ */
	MovableObject() {};

	/* デストラクタ */
	~MovableObject() {};

	/* スタート処理 */
	bool Start()override;
	/* 更新処理 */
	void Update()override {};
	/* 描画処理 */
	void Render(RenderContext& rc)override {};

};
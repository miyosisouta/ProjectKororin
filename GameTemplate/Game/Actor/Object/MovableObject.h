/**
 * MovableObject.h
 * 動くオブジェクトを管理するクラス
 */
#pragma once
#include "StageObjectBase.h"


class MovableObject : public StageObjectBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	MovableObject() {};
	/**
	 * @brief デストラクタ
	 */
	~MovableObject() {};


	bool Start() override;
};
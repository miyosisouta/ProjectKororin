/**
 * @file UICamera.h
 * @brief UI用のカメラを表すクラスの宣言。
 */
#pragma once
#include "Camera/CameraBase.h"

/**
 * @brief UI描画用のカメラオブジェクト。
 */
class UICamera : public CameraBase
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	UICamera() {};
	/**
	 * @brief デストラクタ。
	 */
	~UICamera() {};


	/**
	 * @brief 初期化処理。
	 * @return 成功した場合は true。
	 */
	bool Start();
	/**
	 * @brief 毎フレームの更新処理。
	 */
	void Update();
	/**
	 * @brief 描画処理。
	 * @param rc 描画に使用するレンダーコンテキスト。
	 */
	void Render(RenderContext& rc);
};


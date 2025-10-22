/**
 * @file SphereCamera.h
 * @brief 球体に追従するカメラの宣言。
 */
#pragma once
#include "Camera/CameraBase.h"

class Sphere;
class SphereCamera : public CameraBase
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	SphereCamera() {};
	/**
	 * @brief デストラクタ。
	 */
	~SphereCamera() {};


	/**
	 * @brief 初期化処理。
	 * @return 成功した場合は true。
	 */
	bool Start() override;
	/**
	 * @brief 毎フレームの更新処理。
	 */
	void Update() override;


private:
	Sphere* m_sphere_ = nullptr; //!< 追従対象の球体
};


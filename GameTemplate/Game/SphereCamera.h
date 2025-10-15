/**
 * @file SphereCamera.h
 * @brief 球体に追従するカメラの宣言。
 */
#pragma once


class Sphere;
class SphereCamera : public IGameObject
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
	bool Start();
	/**
	 * @brief 毎フレームの更新処理。
	 */
	void Update();


private:
	Sphere* m_sphere_ = nullptr; //!< 追従対象の球体
	Vector3 m_cameraPos_ = Vector3::Zero; //!< カメラのワールド座標
};


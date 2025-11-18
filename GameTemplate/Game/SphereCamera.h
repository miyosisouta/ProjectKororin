/**
 * @file SphereCamera.h
 * @brief 球体に追従するカメラの宣言。
 */
#pragma once


class Sphere;
class SphereCamera : public IGameObject
{
private:
	Sphere* sphere_ = nullptr; //!< 追従対象の球体
	Transform transform_; //!< トランスフォーム


private:
	Vector3 lerpCurrentMovement_ = Vector3::Zero;
	Vector3 beforeFrameMovement_ = Vector3::Zero;
	Vector3 nextPosition = Vector3::Zero; // カメラが必要レベルに達したときに移動する次の座標
	uint8_t nextNeedLevel = 2; // カメラを下げるときに必要な塊のレベル

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


public:
	/* Sphereの情報をCameraにSetする */
	inline void SetTarget(Sphere* sphere) { sphere_ = sphere; }


private:
	/* 塊がレベルアップしたかどうかのフラグ */
	bool CheckForLevelUp(uint8_t currentLevel) 
	{ 
		if(nextNeedLevel == currentLevel)
		{			
			return true;
		}
		return false;
	}

	/* 次に移動させるカメラの座標の設定 */
	void InitZoomOut();
	/* ズームアウトの計算 */
	void CalcZoomOut();
};


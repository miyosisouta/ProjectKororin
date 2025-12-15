/**
 * @file SphereCamera.h
 * @brief 球体に追従するカメラの宣言。
 */
#pragma once


class Sphere;
class SphereCamera : public IGameObject
{
protected:
	ModelRender* model_;
	Sphere* sphere_ = nullptr; //!< 追従対象の球体
	Transform transform_; //!< トランスフォーム


private:
	Vector3 lerpCurrentMovement_ = Vector3::Zero; // lerp後のフレームの移動量
	Vector3 beforeFrameMovement_ = Vector3::Zero; // lerp前のフレームの移動量
	Vector3 nextPosition_ = Vector3::Zero; // カメラが必要レベルに達したときに移動する次の座標
	uint8_t nextNeedLevel_ = 2; // カメラを下げるときに必要な塊のレベル
	bool isActive_ = true; // 動かしていいかどうか
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
	/* 座標の設定 */
	inline void SetPosition(Vector3 pos) { transform_.m_localPosition = pos; }
	/* 位置情報を取得 */
	inline Vector3& GetPosition() { return transform_.m_position; }

	/* Sphereの情報をCameraにSetする */
	inline void SetTarget(Sphere* sphere) { sphere_ = sphere; }
	/* カメラの視点をモデルに設定 */
	inline void SetResultTarget(ModelRender* target) { model_ = target; }
	
	/* カメラを動かしてよいかの設定 */
	inline void SetCameraActive(bool isActived) { isActive_ = isActived; }
	/* カメラがアクティブ状態かの確認用関数 */
	inline bool IsActiveCamera() { return isActive_; }

private:
	/* 塊がレベルアップしたかどうかのフラグ */
	bool CheckForLevelUp(uint8_t currentLevel) 
	{ 
		if(nextNeedLevel_ == currentLevel)
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

class ResultCamera : public SphereCamera 
{
public:
	ResultCamera() {};
	~ResultCamera() {};

	bool Start() override;
	void Update() override {}
	void Render(RenderContext& rc) override {};
};

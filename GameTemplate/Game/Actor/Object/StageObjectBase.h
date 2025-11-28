/**
 * StageObjectBase.h
 * ステージに配置されるオブジェクト1つを処理するクラス
 */
#pragma once
#include "Actor/Actor.h"


class StageObjectBase : public Actor
{
protected:
	Vector3 colliderPivot_; // コライダーの初期位置
	Vector3 colliderSize_; // コライダーの大きさ
	std::string assetName_; // アセットの名前
	uint8_t attachableValue_; // 吸着可能かどうか
	uint8_t objectSize_; // 吸着可能サイズ
	uint8_t grouthAmount_; // 塊のサイズの増加量

	// 静的な当たり判定(物理的に当たったということをしたい用)
	PhysicsStaticObject* physicsStaticObject_ = nullptr;
	// ゴーストな当たり判定(物理的にあたらない)
	CollisionObject* collisionObject_ = nullptr;
	// ICollisionインターフェース型を指すユニークポインタ
	std::unique_ptr<BoxCollision> collider_;

	bool isVisible_ = true;


public:
	/**
	 * @brief コンストラクタ
	 */
	StageObjectBase() {};
	/**
	 * @brief デストラクタ
	 */
	~StageObjectBase() {};

	virtual bool Start() override;
	virtual void Update() override {};
	virtual void Render(RenderContext& rc) override {};

	
public:
	inline PhysicsStaticObject* GetPhysicsStaticObject() { return physicsStaticObject_; }
	inline const Vector3 GetSize() { return size_; }
	inline const Vector3 GetPosition() { return position_; }


public:
	/**
	 * @brief 初期化
	 * @note  NewGOした直後に絶対呼んでください！！！！
	 */
	void Initialize(const int attachValue_, const Vector3& position, const Vector3& scale, const Quaternion& rotation, const int size, const std::string& assetName, const uint8_t grouthAmount, const Vector3& colliderPivot, const Vector3& colliderSize)
	{
		transform_.m_localPosition = position; // 座標
		transform_.m_localScale = scale; // 大きさ
		transform_.m_rotation = rotation; // 回転

		attachableValue_ = attachValue_; // 吸着可能かどうか
		objectSize_ = size; // オブジェクトの吸着可能サイズ
		assetName_ = assetName; // アセットの名前
		grouthAmount_ = grouthAmount; // 塊のサイズの増加量
		colliderPivot_ = colliderPivot; // コライダーの起点の座標
		colliderSize_ = colliderSize; // コライダーの大きさ

	}


public:
	inline int GetObjectSize() 
	{
		return objectSize_;
	}

	void SetVisible(const bool flg) { isVisible_ = flg; }


private:
	/* オブジェクトのスケールとコリジョンのサイズで出たスケールの差異を求める */
	Vector3 ApplyCollisionSizeDelta(Vector3 scale);

	/* 求めた差異を考慮された値を乗算 */
	Vector3 Multply(Vector3 a, Vector3 b);

private:
	Vector3 size_ = Vector3::One;
	Vector3 position_ = Vector3::Zero;
};
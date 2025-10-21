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
	int attachableValue_; // 吸着可能かどうか
	int objectSize_; // 吸着可能サイズ
	int grouthAmount_; // 塊のサイズの増加量

	// 静的な当たり判定(物理的に当たったということをしたい用)
	PhysicsStaticObject* physicsStaticObject_ = nullptr;
	// ゴーストな当たり判定(物理的にあたらない)
	CollisionObject* collisionObject_ = nullptr;


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
	// @todo for test
	inline PhysicsStaticObject* GetPhysicsStaticObject()
	{
		return physicsStaticObject_;
	}


public:
	/**
	 * @brief 初期化
	 * @note  NewGOした直後に絶対呼んでください！！！！
	 */
	void Initialize(const int attachValue_, const Vector3& position, const Vector3& scale, const Quaternion& rotation, const float size, const std::string& assetName, const int grouthAmount, const Vector3& colliderPivot, const Vector3& colliderSize)
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


private:
	/* オブジェクトのスケールとコリジョンのサイズで出たスケールの差異を求める */
	Vector3 ApplyCollisionSizeDelta(Vector3 scale);

	/* 求めた差異を考慮された値を乗算 */
	Vector3 Multply(Vector3 a, Vector3 b);
};
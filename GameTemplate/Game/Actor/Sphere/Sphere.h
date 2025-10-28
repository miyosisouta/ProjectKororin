/**
 * Sphere.h
 * 塊を表すクラスの宣言
 */

#pragma once
#include "Actor/Actor.h"
class AttachableObject;

class Sphere : public Actor
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	Sphere() {}
	/**
	 * @brief デストラクタ。
	 */
	~Sphere() {}


	/**
	 * @brief 初期化処理。
	 * @return 成功した場合は true。
	 */
	virtual bool Start() override;
	/**
	 * @brief 毎フレームの更新処理。
	 */
	virtual void Update() override;
	/**
	 * @brief 描画処理。
	 * @param rc 描画に使用するレンダーコンテキスト。
	 */
	virtual void Render(RenderContext& rc) override;

	/**
	 * @brief transformの親子関係の設定
	 */
	void SetParent(AttachableObject* attachableObject);


public:
	/**
	 * @brief 移動方向を設定します。
	 * @param direction 設定する移動方向を表す Vector3 型の値。
	 */
	inline void SetMoveDirection(const Vector3& direction)
	{
		moveDirection_ = direction;
	}

	/**
	 * @brief 球体の半径を取得します。
	 * @return 半径。
	 */
	inline const float GetRadius() const
	{
		return radius_;
	}


	/**
	 * @brief 球体のサイズレベルを設定します。
	 * @return 設定された球体のサイズレベルを表す整数値。
	 */
	inline int SetSphereSizeLevel(const int level)
	{
		sizeLevel_ = level;
	}
	/**
	 * @brief 球体のサイズレベルを取得します。
	 * @return 現在のサイズレベル（int型）を返します。
	 */
	inline const int GetSphereSizeLevel() 
	{
		return sizeLevel_;
	}
	
	inline int GrowByRadius(int grouthAmount) 
	{
		radius_ += grouthAmount;

		// @todo for test
		return 0;
	}

public:
	/**
	 * @brief コライダーを取得します。
	 * @return m_collider メンバーへのポインタ。
	 */
	inline CCompoundCollider* GetCollider()
	{
		return m_collider;
	}
	/**
	 * @brief 衝突オブジェクトを取得します。
	 * @return collisionObject_ ポインタを返します。
	 */
	inline CollisionObject* GetCollisionObject()
	{
		return collisionObject_;
	}



private:
	/**
	 * @brief 移動処理。
	 */
	void Move();

	/**
	 * @brief 回転処理
	 */
	void Rotation();

	/**
	 * @brief 重力設定処理。
	 */
	void SetGravity();


private: // Sphere関係の変数

	ModelRender sphereRender_; //!< 球体モデル描画用
	//CharacterController charaCon_; //!< キャラクターコントローラ
	CCompoundCollider* m_collider = nullptr;	//!< 当たり判定(子どもに追加していけるやつ)
	RigidBody* m_rigidBody = nullptr;			//!< 剛体(物理空間での判定に必要)

	Vector3 moveSpeed_ = Vector3::Zero; //!< 移動速度
	Vector3 moveDirection_ = Vector3::Zero; //!< 移動方向
	Vector3 beforePosition_ = Vector3::Zero; //!< 前の座標を保存
	Vector3 vertical_ = Vector3::Zero; // 外積
	int sizeLevel_ = 1; //!< 塊のサイズ : 吸着可能なオブジェクトのサイズ
	float moveSpeedMultiply_ = 0.0f; //!< 移動速度乗算
	float radius_ = 1.0f; //!< 半径

	CollisionObject* collisionObject_ = nullptr; //衝突判定オブジェクト

};


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
	Sphere() {};
	/**
	 * @brief デストラクタ。
	 */
	~Sphere() {};


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

	/**
	 * @brief transformの親子関係の設定
	 */
	void SetParent(AttachableObject* attachableObject);


public:
	/**
	 * @brief 移動方向を設定します。
	 * @param direction 設定する移動方向を表す Vector3 型の値。
	 */
	void SetMoveDirection(const Vector3& direction)
	{
		moveDirection_ = direction;
	}

	/**
	 * @brief 球体の半径を取得します。
	 * @return 半径。
	 */
	const float GetRadius() const
	{
		return radius_;
	}


public:
	inline CCompoundCollider* GetCollider()
	{
		return m_collider;
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
	Vector3 vertical_ = Vector3::Zero;
	float moveSpeedMultiply_ = 0.0f; //!< 移動速度乗算
	float radius_ = 1.0f; //!< 半径

	CollisionObject* collisionObject_ = nullptr; //衝突判定オブジェクト

};


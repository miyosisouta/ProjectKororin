/**
 * Sphere.h
 * 塊を表すクラスの宣言
 */

#pragma once
#include "Actor.h"
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

	/**
	 * @brief レイをヒットさせる処理を実行します。
	 */
	void HitRay();


private: // Sphere関係の変数

	ModelRender sphereRender_; //!< 球体モデル描画用
	CharacterController charaCon_; //!< キャラクターコントローラ

	Vector3 moveSpeed_ = Vector3::Zero; //!< 移動速度
	Vector3 moveDirection_ = Vector3::Zero; //!< 移動方向
	Vector3 beforePosition_ = Vector3::Zero; //!< 前の座標を保存
	Vector3 vertical_ = Vector3::Zero;
	float moveSpeedMultiply_ = 0.0f; //!< 移動速度乗算
	float radius_ = 1.0f; //!< 半径

	CollisionObject* collisionObject_ = nullptr; //衝突判定オブジェクト

private: // Ray関係の変数

	PhysicsWorld* physicsWorld_ = nullptr;
	Vector3 rayStarts_; //!< レイ開始点
	Vector3 rayDirections = Vector3(1, 0, 0); //!< レイ方向
	float rayLength_ = 0.0f; //!<レイの長さ

};


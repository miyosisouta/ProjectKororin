/**
 * Sphere.h
 * 塊を表すクラスの宣言
 */

#pragma once
#include "Actor/Actor.h"


class AttachableObject;


class SphereStatus
{
private:
	int levelUpNum_ = 0;	//!< レベルアップに必要な数(吸着したオブジェクトの数)
	int currentLevel = 1;	//!< 現在の吸着できるレベル(初期値は1)


public:
	/* コンストラクタ */
	SphereStatus();
	/* デストラクタ */
	~SphereStatus();

public:
	/* レベルアップに必要な吸着したオブジェクトの数の取得 */
	int GetLevelUpNum() const { return levelUpNum_; }

	/* 現在のレベルを1増やす */
	void AddLevel() { ++currentLevel; }

	/* 現在のレベルを取得 */
	int GetLevel() { return currentLevel; }

public:
	/**
	 * 対象のパラメーターを受け取って設定する
	 * NOTE:初期設定やレベルアップしたときに呼ばれる
	 */
	void Setup(const MasterSphereStatusParameter& parameter);
};




/**************************/


class Sphere : public Actor
{
public:
	/* コンストラクタ */
	Sphere() {}
	/* デストラクタ */
	~Sphere();

	/* スタート処理 */
	virtual bool Start() override;
	/* 更新処理 */
	virtual void Update() override;
	/* 描画処理 */
	virtual void Render(RenderContext& rc) override;

	/* トランスフォームの親子関係の設定 */
	void SetParent(AttachableObject* attachableObject);


public:
	/* 移動方向の設定 */
	inline void SetMoveDirection(const Vector3& direction) { moveDirection_ = direction; }

	/* 球体の半径の取得 */
	inline const float GetRadius() { return radius_; }

	/* 機能に影響を与える半径 */
	inline const float GetEffectiveRadius() { return effectiveRadius_; }

	/* 球体のサイズレベルの設定 */
	inline const int SetSphereSizeLevel(const int level) { sizeLevel_ = level; }

	/* 球体のサイズレベルの取得 */
	inline const int GetSphereSizeLevel() { return sizeLevel_; }

	/* 座標の取得 */
	inline const Vector3& GetPosition() { return transform_.m_position; }

	/* 座標の設定 */
	inline const void SetPosition(const Vector3 pos) { transform_.m_localPosition = pos; }

	/* 塊の描画フラグの設定 */
	inline const void SetIsDraw(const bool isDrawed) { isDraw_ = isDrawed; }

	/* 塊の描画フラグの取得 */
	inline const bool GetIsDraw() { return isDraw_; }

	/* 塊のY座標を上げる */
	inline const void AddPositionY(const float posY) { transform_.m_localPosition.y = posY; }

	/* 塊の半径と影響を与える半径のサイズアップ */
	inline int GrowByRadius(int grouthAmount)
	{
		effectiveRadius_ += grouthAmount / 5.0f;
		radius_ += grouthAmount;
		return 0;
	}

	/* 外部から与える力の設定 */
	inline void AddForce(Vector3 force) { addForce_ = force; }


public:
	/* コライダーを取得 */
	inline SphereCollision* GetCollider() { return collider_.get(); }

	/** ステータス取得 */
	SphereStatus* GetStatus() { return status_.get(); }

	/* 吸着したオブジェクトの数を増やす */
	void AddCurrentLevelUpNum() { ++currentLevelUpNum_; }

	/* オブジェクトが引っ付いた合計数を増やす */
	void AddTotalNum() { ++totalNum_; }

	/* 引っ付いたオブジェクトの数を取得 */
	int GetTotalNum() { return totalNum_; }

	/* 目標サイズを達成したかどうか */
	bool CheakGoalSize();

	/* 吸着したオブジェクトをリストにまとめる */
	void AddAttachableObject(AttachableObject* object) { m_attachableObjectList.push_back(object); }

	/* 動かせるようにするか設定 */
	void SetPlayable(const bool flg) { isPlayable_ = flg; }


private:
	/* 移動処理 */
	void Move();

	/* 回転処理 */
	void Rotation();


	/* レベルアップチェック用の関数 */
	void UpdateLevelUp(const bool isInit = false);


private:

	ModelRender sphereRender_; //!< 球体モデル描画用
	CCompoundCollider* m_collider = nullptr; //!< 当たり判定(子どもに追加していけるやつ)
	RigidBody* m_rigidBody = nullptr; //!< 剛体(物理空間での判定に必要)

	Vector3 moveSpeed_ = Vector3::Zero; //!< 移動速度
	Vector3 moveDirection_ = Vector3::Zero; //!< 移動方向
	Vector3 beforePosition_ = Vector3::Zero; //!< 前の座標を保存
	Vector3 vertical_ = Vector3::Zero; // 外積
	Vector3 addForce_; //!< 外部からもらう力
	int sizeLevel_ = 1; //!< 塊のサイズ : 吸着可能なオブジェクトのサイズ
	int currentLevelUpNum_;	//!< レベルアップに必要な巻き込んだ数
	int totalNum_ = 0.0f; //!< 塊に引っ付いたオブジェクトの数
	float moveSpeedMultiply_ = 0.0f; //!< 移動速度乗算
	float effectiveRadius_ = 30.0f; //!< 動くときに反映する半径
	float radius_ = 13.0f; //!< 半径
	bool isDraw_ = true; // 描画するかのフラグ
	bool isPlayable_ = true; // 塊を動かすかどうかのフラグ


	std::unique_ptr<SphereStatus> status_;	//!< 塊のステータス
	std::unique_ptr<SphereCollision> collider_;	//!< 抽象化された衝突オブジェクトを所有・管理するための器
	std::vector<AttachableObject*> m_attachableObjectList; //!< 吸着したオブジェクトのリスト
};


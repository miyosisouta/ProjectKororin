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
	int levelUpNum_ = 0;	// レベルアップに必要な数(吸着したオブジェクトの数)
	int currentLevel = 1;	// 現在の吸着できるレベル(初期値は1)


public:
	SphereStatus();
	~SphereStatus();

	/* レベルアップに必要な吸着したオブジェクトの数の取得 */
	int GetLevelUpNum() const { return levelUpNum_; }
	/* 現在のレベルを1増やす */
	void AddLevel() { ++currentLevel; }
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
	/**
	 * @brief コンストラクタ。
	 */
	Sphere() {}
	/**
	 * @brief デストラクタ。
	 */
	~Sphere();


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
	inline void SetMoveDirection(const Vector3& direction){ moveDirection_ = direction;	}

	/**
	 * @brief 球体の半径を取得します。
	 * @return 半径。
	 */
	inline const float GetRadius() { return radius_; }

	/**
	 * @brief 他の昨日に影響を与える半径を取得
	 * @return 
	 */
	inline const float GetEffectiveRadius() { return effectiveRadius_; }

	/**
	 * @brief 球体のサイズレベルを設定します。
	 * @return 設定された球体のサイズレベルを表す整数値。
	 */
	inline const int SetSphereSizeLevel(const int level){	sizeLevel_ = level;	}

	/**
	 * @brief 球体のサイズレベルを取得します。
	 * @return 現在のサイズレベル（int型）を返します。
	 */
	inline const int GetSphereSizeLevel() {	return sizeLevel_; }
	
	/**
	 * @brief 球体のポジションを取得
	 * @return 
	 */
	inline const Vector3& GetPosition() { return transform_.m_position; }

	/**
	 * @brief 球体のポジションを設定
	 * @param pos 
	 * @return 
	 */
	inline const void SetPosition(const Vector3 pos) { transform_.m_localPosition = pos; }

	inline const void SetIsDraw(const bool isDrawed) { isDraw_ = isDrawed; }
	inline const bool GetIsDraw() { return isDraw_; }
	/**
	 * @brief 球体のY座標を増加させる
	 * @param posY 
	 * @return 
	 */
	inline const void AddPositionY(const float posY){ transform_.m_localPosition.y = posY; }
	
	/**
	 * @brief 塊のサイズを成長させる
	 * @param grouthAmount オブジェクトの大きさ
	 */
	inline int GrowByRadius(int grouthAmount) 
	{
		effectiveRadius_ += grouthAmount / 5.0f;
		radius_ += grouthAmount;
		return 0;
	}

	/**
	 * @brief 外部から力を与える
	 * @param force 
	 */
	inline void AddForce(Vector3 force) { m_addForce = force; }

public:
	/**
	 * @brief コライダーを取得します。
	 * @return m_collider メンバーへのポインタ。
	 */
	inline SphereCollision* GetCollider() { return collider_.get(); }


	/** ステータス取得 */
	SphereStatus* GetStatus() { return status_.get(); }


	/* 吸着したオブジェクトの数を増やす */
	void AddCurrentLevelUpNum() { ++currentLevelUpNum_; }

	/* オブジェクトが引っ付いた合計数を増やす */
	void AddTotalNum() { ++totalNum_; }

	/* 引っ付いたオブジェクトの数を取得 */
	int GetTotalNum() { return totalNum_; }

	/**
	 * @brief ゴールサイズに達したか
	 */
	bool CheakGoalSize();

	void AddAttachableObject(AttachableObject* object)
	{
		m_attachableObjectList.push_back(object);
	}


	void SetPlayable(const bool flg) { isPlayable_ = flg; }



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
	 * @brief レベルアップチェック用の関数
	 */
	void UpdateLevelUp(const bool isInit = false);


private: // Sphere関係の変数

	ModelRender sphereRender_; //!< 球体モデル描画用
	CCompoundCollider* m_collider = nullptr;	//!< 当たり判定(子どもに追加していけるやつ)
	RigidBody* m_rigidBody = nullptr;			//!< 剛体(物理空間での判定に必要)

	Vector3 moveSpeed_ = Vector3::Zero; //!< 移動速度
	Vector3 moveDirection_ = Vector3::Zero; //!< 移動方向
	Vector3 beforePosition_ = Vector3::Zero; //!< 前の座標を保存
	Vector3 vertical_ = Vector3::Zero; // 外積
	int sizeLevel_ = 1; //!< 塊のサイズ : 吸着可能なオブジェクトのサイズ
	int currentLevelUpNum_;	//!< レベルアップに必要な巻き込んだ数
	int totalNum_ = 0.0f; //!< 塊に引っ付いたオブジェクトの数
	float moveSpeedMultiply_ = 0.0f; //!< 移動速度乗算
	float effectiveRadius_ = 30.0f; //!< 動くときに反映する半径
	float radius_ = 13.0f; //!< 半径
	bool isDraw_ = true;

	bool isPlayable_ = true;

	//CollisionObject* collisionObject_ = nullptr; //衝突判定オブジェクト
	//SphereCollision* sphereCollision_ = nullptr; // 衝突判定ゴーストオブジェクト

	std::unique_ptr<SphereStatus> status_;	//!< ステータス
	std::unique_ptr<SphereCollision> collider_;	//!< 抽象化された衝突オブジェクトを所有・管理するための器

	std::vector<AttachableObject*> m_attachableObjectList;


	Vector3 m_addForce;							//!< 外部からもらう力
};


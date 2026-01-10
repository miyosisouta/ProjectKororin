/**
 * StageObjectBase.h
 * ステージに配置されるオブジェクト1つを処理するクラス
 */
#pragma once
#include "Actor/Actor.h"

struct ObjectData
{
	std::string	assetName		= "";
	Vector3		position		= Vector3::Zero;		// 座標
	Quaternion	rotation		= Quaternion::Identity; // 回転
	Vector3		scale			= Vector3::One;			// 大きさ
	int			attachValue		= 0;					// 吸着可能かどうか
	int			size			= 0;					// オブジェクトの吸着可能サイズ
	uint8_t		grouthAmount	= 0;					// 塊のサイズの増加量
	Vector3		colliderPivot	= Vector3::Zero;		// コライダーの起点の座標
	Vector3		colliderSize	= Vector3::Zero;		// コライダーの大きさ
	float		uiObjectScal	= 0.0f;					// UIとして表示する際のオブジェクトの大きさの倍率
	int			soundNum		= 0;					//効果音の番号
};

class StageObjectBase : public Actor
{
protected:
	Vector3 colliderPivot_; // コライダーの初期位置
	Vector3 colliderSize_; // コライダーの大きさ
	std::string assetName_; // アセットの名前
	uint8_t attachableValue_; // 吸着可能かどうか
	uint8_t objectSize_; // 吸着可能サイズ
	int grouthAmount_; // 塊のサイズの増加量
	float UIDisplayscale_; // UIとして表示するときのオブジェクトの大きさの倍率
	int	soundNum_;//効果音の番号

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
	inline const Vector3& GetSize() { return size_; }
	inline const Vector3& GetPosition() { return position_; }
	inline const Vector3& GetColliderSize() { return colliderSize_; }
	inline void OffRender() { isVisible_ = false; }

public:
	/**
	 * @brief 初期化
	 * @note  NewGOした直後に絶対呼んでください！！！！
	 */
	void Initialize(ObjectData* objectData);

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
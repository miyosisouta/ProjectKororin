/**
 * StageObjectBase.h
 * ステージに配置されるオブジェクト1つを処理するクラス
 */
#pragma once
#include "Actor/Actor.h"

struct ObjectData
{
	std::string	assetName = "";						// アセットの名前
	Vector3		position = Vector3::Zero;			// 座標
	Quaternion	rotation = Quaternion::Identity;	// 回転
	Vector3		scale = Vector3::One;				// 大きさ
	int			attachValue = 0;					// オブジェクトタイプ
	int			size = 0;							// オブジェクトの吸着可能サイズレベル
	uint8_t		grouthAmount = 0;					// 塊のサイズの増加量
	Vector3		colliderPivot = Vector3::Zero;		// コライダーの起点の座標
	Vector3		colliderSize = Vector3::Zero;		// コライダーの大きさ
	float		uiObjectScal = 0.0f;				// UIとして表示する際のオブジェクトの大きさの倍率
	int			soundNum = 0;						// SEの設定番号
};

class StageObjectBase : public Actor
{
protected:
	Vector3 colliderPivot_;		//!< コライダーの初期位置
	Vector3 colliderSize_;		//!< コライダーの大きさ
	std::string assetName_;		//!< アセットの名前
	uint8_t attachableValue_;	//!< 吸着タイプ
	uint8_t objectSize_;		//!< 吸着可能サイズ
	int grouthAmount_;			//!< 塊のサイズの増加量
	float UIDisplayscale_;		//!< UIとして表示するときのオブジェクトの大きさの倍率
	int	soundNum_;				//!< 効果音の番号

	PhysicsStaticObject* physicsStaticObject_ = nullptr; //!< 静的な当たり判定(物理的に当たったということをしたい用)
	CollisionObject* collisionObject_ = nullptr; //!< ゴーストな当たり判定(物理的にあたらない)
	std::unique_ptr<BoxCollision> collider_; //!< ICollisionインターフェース型を指すユニークポインタ

	bool isVisible_ = true; //!< 描画するかフラグ


public:
	/* コンストラクタ */
	StageObjectBase() {};
	/* デストラクタ */
	~StageObjectBase() {};

	/* 初期化処理 */
	virtual bool Start() override;
	/* 更新処理 */
	virtual void Update() override {};
	/* 描画処理 */
	virtual void Render(RenderContext& rc) override {};


public:
	/* 静的オブジェクトの取得 */
	inline PhysicsStaticObject* GetPhysicsStaticObject() { return physicsStaticObject_; }

	/* 座標の取得 */
	inline const Vector3& GetPosition() { return position_; }

	/* BoxCollier用サイズの取得 */
	inline const Vector3& GetSize() { return boxColliderScale_; }

	/* コライダーのサイズを取得 */
	inline const Vector3& GetColliderSize() { return colliderSize_; }

	/* オブジェクトの描画を止める */
	inline void OffRender() { isVisible_ = false; }

	/* オブジェクトを描画するか設定 */
	void SetVisible(const bool flg) { isVisible_ = flg; }


public:
	/* jsonファイルの情報を設定 */
	void Initialize(ObjectData* objectData);


private:
	/* オブジェクトのスケールとコリジョンのサイズで出たスケールの差異を求める */
	Vector3 ApplyCollisionSizeDelta(Vector3 scale);

	/* 求めた差異を考慮された値を乗算 */
	Vector3 Multply(Vector3 a, Vector3 b);

private:
	Vector3 boxColliderScale_ = Vector3::One; //!< ボックスコライダーのサイズ用変数
	Vector3 position_ = Vector3::Zero; //!< 座標
};
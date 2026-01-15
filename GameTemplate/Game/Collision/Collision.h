/**
 * CollisionManager.h
 * 当たり判定管理
 * 衝突判定の簡略化と高速化
 */
#pragma once


class AABBBox
{
private:
	Vector3 min_ = Vector3::Zero; // AABBの最小座標
	Vector3 max_ = Vector3::Zero; // AABBの最大座標

public:
	/**
	 * 1. 空のAABBを作成するデフォルトコンストラクタ
	 */
	AABBBox() {}

	/**
	 * 2. 中心座標とサイズ（半幅/半高/半奥行き）を渡してAABBを作成する
	 * @param center 中心座標
	 * @param size_half 各軸の長さの半分 (例: {幅/2, 高さ/2, 奥行き/2})
	 */
	AABBBox(const Vector3& center, const Vector3& size_half)
	{
		min_ = center - size_half;
		max_ = center + size_half;
	}

	/**
	 * 3. 中心座標と半径を渡して球体を内包する最小のAABBを作成する
	 * この場合、size_halfは {radius, radius, radius} となる
	 * @param center 球の中心座標
	 * @param radius 球の半径
	 */
	AABBBox(const Vector3& center, float radius)
	{
		Vector3 r_vec = Vector3(radius);
		min_ = center - r_vec;
		max_ = center + r_vec;
	}

	// -----------------------------------------------------------------
	// 当たり判定関数
	// -----------------------------------------------------------------

	/**
	 * 4. 引数で渡されたAABBとこのAABBが衝突しているかを判定する
	 * AABB同士の衝突判定は、各軸(x, y, z)で重なりがあるかをチェックする
	 * @param other 比較対象のAABB
	 * @return 衝突していれば true
	 */
	bool IsHit(const AABBBox& other) const
	{
		if (min_.x > other.max_.x) return false;
		if (max_.x < other.min_.x) return false;
		//if (min_.y > other.max_.y) return false;
		//if (max_.y < other.min_.y) return false;
		if (min_.z > other.max_.z) return false;
		if (max_.z < other.min_.z) return false;
		return true;
	}
};


/* コリジョンタイプ */
struct CollisionType
{
	enum Enum
	{
		Sphere,
		Box
	};
};

/* アイコリジョン */
class ICollision
{
protected:
	CollisionType::Enum type_ = CollisionType::Box; //!< コリジョンタイプ
	Vector3 position_ = Vector3::Zero;				//!< 座標
	AABBBox aabb_;									//!< AABB


public:
	/* コンストラクタ */
	ICollision() {}
	/* デストラクタ */
	virtual ~ICollision() {}

	/* 更新処理 */
	virtual void Update() = 0;

	/* 衝突判定 */
	virtual bool IsHit(ICollision* other) = 0;

	/* コリジョンタイプの取得 */
	CollisionType::Enum GetCollisionType() { return type_; }

	/* 座標の設定 */
	void SetPosition(const Vector3& pos) { position_ = pos; }

	/* 座標の取得 */
	const Vector3& GetPosition() const { return position_; }

	/* AABBのデータを取得 */
	const AABBBox& GetAABB() const { return aabb_; }
};

/************************** 球用コリジョン *************************/
class SphereCollision : public ICollision
{
private:
	float radius_ = 0.0f; //!< 半径


public:
	/* コンストラクタ */
	SphereCollision() {}
	/* デストラクタ */
	~SphereCollision() {}

	/* 初期化処理 */
	void Init(const Vector3 pos, const float radius);
	/* 更新処理 */
	void Update() override;
	/* 当たったかどうかフラグ */
	bool IsHit(ICollision* other) override;


public:
	/* 半径を取得 */
	float GetRadius() const { return radius_; }
};

/************************** ボックス用コリジョン *************************/

class BoxCollision : public ICollision
{
private:
	Vector3 size_ = Vector3::Zero; //!< 大きさ


public:
	/* コンストラクタ */
	BoxCollision() {};
	/* デストラクタ */
	~BoxCollision() {};

	/* 初期化処理 */
	void Init(const Vector3 pos, const Vector3 size);
	/* 更新処理 */
	void Update() override;
	/* 当たったかどうかフラグ */
	bool IsHit(ICollision* other) override;

public:
	Vector3 GetSize() const { return size_; }
};

/************************** テンプレート *************************/

// T: 引数の型（float, intなど）
template <typename T>
T ClampValue(const T& value, const T& min_val, const T& max_val)
{
	// value < min_val なら min_val を返す
	if (value < min_val) {
		return min_val;
	}
	// value > max_val なら max_val を返す
	if (value > max_val) {
		return max_val;
	}

	// 範囲内であればそのまま value を返す
	return value;
}
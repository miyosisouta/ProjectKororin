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
     * 2. min/max座標を直接指定してAABBを作成するコンストラクタ
     */
    //AABBBox(const Vector3& min_p, const Vector3& max_p) : min_(min_p), max_(max_p) {}

    /**
     * 3. 中心座標とサイズ（半幅/半高/半奥行き）を渡してAABBを作成する
     * @param center 中心座標
     * @param size_half 各軸の長さの半分 (例: {幅/2, 高さ/2, 奥行き/2})
     */
    AABBBox(const Vector3& center, const Vector3& size_half)
    {
        // min = center - size_half
        min_ = center - size_half;
        // max = center + size_half
        max_ = center + size_half;
    }

    /**
     * 4. 中心座標と半径を渡して球体を内包する最小のAABBを作成する
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
     * 5. 引数で渡されたAABBとこのAABBが衝突しているかを判定する
     * AABB同士の衝突判定は、各軸(x, y, z)で重なりがあるかをチェックする
     * @param other 比較対象のAABB
     * @return 衝突していれば true
     */
    bool IsHit(const AABBBox& other) const
    {
        if (min_.x > other.max_.x) return false;
        if (max_.x < other.min_.x) return false;
        if (min_.y > other.max_.y) return false;
        if (max_.y < other.min_.y) return false;
        if (min_.z > other.max_.z) return false;
        if (max_.z < other.min_.z) return false;
        return true;
    }
};


struct CollisionType
{
	enum Enum
	{
		Sphere,
		Box
	};
};


class ICollision
{
protected:
	CollisionType::Enum type_ = CollisionType::Box;
	Vector3 position_ = Vector3::Zero;


public:
	ICollision() {}
	virtual ~ICollision() {}

    /* 衝突判定 */
	virtual bool IsHit(ICollision* other) = 0;

	CollisionType::Enum GetCollisionType() { return type_; }
	void SetPosition(const Vector3& pos) { position_ = pos; }
	const Vector3& GetPosition() const { return position_; }
};


class SphereCollision : public ICollision
{
private:
	float radius_ = 0.0f;


public:
	SphereCollision() {}
	~SphereCollision() {}


	void Init(const Vector3 pos, const float radius);
	bool IsHit(ICollision* other) override;


	float GetRadius() const { return radius_; }
};


class BoxCollision : public ICollision
{
private:
    Vector3 size_ = Vector3::Zero;


public:
    BoxCollision() {};
    ~BoxCollision() {};

    void Init(const Vector3 pos, const Vector3 size);
    bool IsHit(ICollision* other)override;

    Vector3 GetSize() const { return size_; }
};
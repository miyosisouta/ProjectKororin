/**
 * CollisionManager.h
 * 当たり判定管理
 */
#pragma once


class Sphere; // 塊
class StageObjectBase; // オブジェクト管理
class AABBBox;



/* 情報の管理用構造体 */
struct CollisionInfo : public Noncopyable
{
	int type;				//!< オブジェクトの種類(プレイヤーと壁がヒットしたみたいな処理をするために必要。自分がだれかの判断)
	IGameObject* object;	//!< 当たり判定を持つオブジェクトのポインタ
	ICollision* collision;	//!< アイコリジョン

	/* オブジェクトとコリジョンの設定 */
	CollisionInfo() : type(GameObjectType::Sphere), object(nullptr), collision(nullptr) {}
	CollisionInfo(const GameObjectType::Enum type, IGameObject* object, ICollision* collision) : type(type), object(object), collision(collision) {}
};



/* 衝突したペアを管理する構造体 */
struct CollisionPair : public Noncopyable
{
	CollisionInfo* left;	//!< 当たり判定A
	CollisionInfo* right;	//!< 当たり判定B
	//
	CollisionPair() : left(nullptr), right(nullptr) {}
	CollisionPair(CollisionInfo* left, CollisionInfo* right) : left(left), right(right) {}
};



// 空間分割クラス
class SplitSpace : public Noncopyable
{
	friend class CollisionHitManager;

private:
	// AABBboxを作る個数
	static const int Z_NUM_ = 16; //!< x方向につくる空間の数
	static const int X_NUM_ = 16; //!< z方向につくる空間の数

	/* 空間分割時に使う変数 */
	float worldSizeX_ = 0.0f; //!< ステージの大きさx座標
	float worldSizeZ_ = 0.0f; //!< ステージの大きさz座標

	float worldSpaceSizeX_ = 0.0f; //!< 1個の空間のサイズx座標
	float worldSpaceSizeZ_ = 0.0f; //!< 1個の空間のサイズz座標

	float worldSpaceHalfSizeX_ = 0.0f; //!< 1個の空間のサイズの半分のサイズx座標
	float worldSpaceHalfSizeZ_ = 0.0f; //!< 1個の空間のサイズの半分のサイズz座標

	float baseX_ = 0.0f; //!< 基準開始地点x座標
	float baseZ_ = 0.0f; //!< 基準開始地点z座標


private:
	/* ステージのサイズの半分を設定 */
	Vector3 worldHalfSize_ = Vector3(500.0f, 0.0f, 500.0f);

	/* 空間分割時の情報 */
	std::vector<CollisionInfo*> colisionSpacialList[X_NUM_][Z_NUM_];



public:
	/* 空間分割時の初期設定 */
	SplitSpace(const Vector3 worldSize)
		: worldHalfSize_(worldSize)		//!<ステージのサイズの設定
	{
		/* ステージ全体のサイズ */
		worldSizeX_ = worldHalfSize_.x * 2.0f;
		worldSizeZ_ = worldHalfSize_.z * 2.0f;

		/* 1個の空間のサイズ */
		worldSpaceSizeX_ = worldSizeX_ / static_cast<float>(X_NUM_);
		worldSpaceSizeZ_ = worldSizeZ_ / static_cast<float>(Z_NUM_);

		/* 1個の空間の半分のサイズ : AABBboxの真ん中の座標を求める */
		worldSpaceHalfSizeX_ = worldSpaceSizeX_ / 2.0f;
		worldSpaceHalfSizeZ_ = worldSpaceSizeZ_ / 2.0f;

		/**
		 * グリッドの左上（または左下）の基準開始位置
		 * worldSpaceHalfSizeX : 作ったAABBboxの中心地に座標を調整
		 * ※ AABBの中心座標として扱いやすいように調整
		 */
		baseX_ = -worldHalfSize_.x + worldSpaceHalfSizeX_;
		baseZ_ = -worldHalfSize_.z + worldSpaceHalfSizeZ_;
	}

	void Update();

	/* 複数ある空間を検索し、その空間にAABBboxを作成する */
	void ForEach(const AABBBox& aabb, const std::function<void(int, int)>& hitFunc);
};




/**
 * 当たり判定処理を一括で行うクラス
 * NOTE:シングルトンでインスタンスを一つにしてアクセスポイントを提供する
 */
class CollisionHitManager
{
private:
	friend class SplitSpace;


private:
	/** 当たり判定を持つ全オブジェクトのリスト */
	std::vector<CollisionInfo*> m_collisionInfoList;
	/** 当たり判定のペア */
	std::vector<std::unique_ptr<CollisionPair>> m_collisionPairList;
	/** 空間分裂のユニークポインタ */
	std::unique_ptr<SplitSpace> splitSpace_;


private:
	// 空間配列用変数 : Sphereがどの空間にいるかを保存する変数
	int sphereGridX_ = 0;
	int sphereGridZ_ = 0;

private:
	/* コンストラクタ */
	CollisionHitManager();
	/* デストラクタ */
	~CollisionHitManager();


public:
	/* 更新処理 */
	void Update();


public:
	/** 判定処理をしたいオブジェクトを登録 */
	//void RegisterCollisionObject(GameObjectType::Enum type, IGameObject* object, CollisionObject* collision);
	void RegisterCollisionObject(GameObjectType::Enum type, IGameObject* object, ICollision* collision);
	void UnregisterCollisionObject(IGameObject* object);


private:
	/* Sphereと吸着可能なオブジェクトが接触したときの処理 */
	bool UpdateHitAttatchableObject(CollisionPair& pair);


private:
	/**
	 * 指定したクラスを取得する
	 * NOTE: 指定したクラスが存在しない場合はnullptrを返す
	 */
	template <typename T>
	T* GetTargetObject(CollisionPair& pair, const GameObjectType::Enum targetType)
	{
		if (pair.left->type == targetType)
		{
			return static_cast<T*>(pair.left->object);
		}
		else if (pair.right->type == targetType)
		{
			return static_cast<T*>(pair.right->object);
		}
		return nullptr;
	}


	/************************* ここからシングルトン **************************/

private:
	// インスタンス
	static CollisionHitManager* instance_;


public:
	/* インスタンスの作成 */
	static void CreateInstance()
	{
		if (instance_ == nullptr)
		{
			instance_ = new CollisionHitManager();
		}
	}

	/* インスタンスの取得 */
	static CollisionHitManager& Get()
	{
		return *instance_;
	}

	/* インスタンスがあるかどうか */
	static bool IsAvailable()
	{
		return instance_ != nullptr;
	}

	/* インスタンスの削除 */
	static void Delete()
	{
		if (instance_)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}
};
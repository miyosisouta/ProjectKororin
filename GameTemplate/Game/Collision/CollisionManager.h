/**
 * CollisionManager.h
 * 当たり判定管理
 */
#pragma once


class Sphere; // 塊
class StageObjectBase; // オブジェクト管理
class AABBBox;




struct CollisionInfo
{
	int type;	// オブジェクトの種類(プレイヤーと壁がヒットしたみたいな処理をするために必要。自分がだれかの判断)
	IGameObject* object;	// 当たり判定を持つオブジェクトのポインタ
	//CollisionObject* collision; // 当たり判定オブジェクトのポインタ
	ICollision* collision;
	//
	CollisionInfo() : type(GameObjectType::Sphere), object(nullptr), collision(nullptr) {}
	//CollisionInfo(const GameObjectType::Enum type, IGameObject* object, CollisionObject* collision) : type(type), object(object), collision(collision) {}
	CollisionInfo(const GameObjectType::Enum type, IGameObject* object, ICollision* collision) : type(type), object(object), collision(collision) {}
};




struct CollisionPair
{
	CollisionInfo* left;	// 当たり判定A
	CollisionInfo* right;	// 当たり判定B
	//
	CollisionPair() : left(nullptr), right(nullptr) {}
	CollisionPair(CollisionInfo* left, CollisionInfo* right) : left(left), right(right) {}
};




class SplitSpace : public Noncopyable
{
	friend class CollisionHitManager;

private:
	static const int X_NUM = 4;
	static const int Z_NUM = 4;

private:
	Vector3 worldHalfSize_ = Vector3(500.0f, 0.0f, 500.0f);

	std::vector<const CollisionInfo*> colisionSpacialList[X_NUM][Z_NUM];



public:
	SplitSpace(const Vector3 worldSize)
		: worldHalfSize_(worldSize)
	{
	}

	void Update();
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
	/** 当たり判定オブジェクトのリスト */
	std::vector<CollisionInfo> m_collisionInfoList;
	/** 当たり判定のペア */
	std::vector<CollisionPair> m_collisionPairList;
	
	std::unique_ptr<SplitSpace> splitSpace_;


private:
	CollisionHitManager();
	~CollisionHitManager();


public:
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

	/* ここから空間分割関係 */

private: 
	///* 検索範囲にあるオブジェクトのデータが入ったListを削除 */
	//void Clear();

	///* ステージ上の吸着可能なオブジェクトの登録 */
	//void Register();
	//
	///* 塊の近くにあるオブジェクトを探して登録 */
	//void RigisterNearObjects();

private:



	/************************* ここからシングルトン **************************/
private:
	static CollisionHitManager* instance_;


public:
	static void CreateInstance()
	{
		if (instance_ == nullptr)
		{
			instance_ = new CollisionHitManager();
		}
	}
	static CollisionHitManager& Get()
	{
		return *instance_;
	}
	static bool IsAvailable()
	{
		return instance_ != nullptr;
	}
	static void Delete()
	{
		if (instance_)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}
};
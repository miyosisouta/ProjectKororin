/**
 * CollisionManager.h
 * 当たり判定管理
 */
#pragma once


class Sphere; // 塊
class StageObjectBase; // オブジェクト管理


struct CollisionInfo
{
	int type;	// オブジェクトの種類(プレイヤーと壁がヒットしたみたいな処理をするために必要。自分がだれかの判断)
	IGameObject* object;	// 当たり判定を持つオブジェクトのポインタ
	CollisionObject* collision; // 当たり判定オブジェクトのポインタ
	//
	CollisionInfo() : type(GameObjectType::Sphere), object(nullptr), collision(nullptr) {}
	CollisionInfo(const GameObjectType::Enum type, IGameObject* object, CollisionObject* collision) : type(type), object(object), collision(collision) {}
};




struct CollisionPair
{
	CollisionInfo* left;	// 当たり判定A
	CollisionInfo* right;	// 当たり判定B
	//
	CollisionPair() : left(nullptr), right(nullptr) {}
	CollisionPair(CollisionInfo* left, CollisionInfo* right) : left(left), right(right) {}
};




/**
 * 当たり判定処理を一括で行うクラス
 * NOTE:シングルトンでインスタンスを一つにしてアクセスポイントを提供する
 */
class CollisionHitManager
{
private:
	/** 当たり判定オブジェクトのリスト */
	std::vector<CollisionInfo> m_collisionInfoList;
	/** 当たり判定のペア */
	std::vector<CollisionPair> m_collisionPairList;
	

private:
	CollisionHitManager();
	~CollisionHitManager();


public:
	void Update();


public:
	/** 判定処理をしたいオブジェクトを登録 */
	void RegisterCollisionObject(GameObjectType::Enum type, IGameObject* object, CollisionObject* collision);
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
	static void Delete()
	{
		if (instance_)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}
};
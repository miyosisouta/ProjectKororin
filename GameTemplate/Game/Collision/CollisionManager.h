/**
 * CollisionManager.h
 * 当たり判定管理
 */
#pragma once


class Sphere; // 塊
class StageObjectBase; // オブジェクト管理
class SoundHitManager;

enum EnCollisionType
{
	enCollisionType_Sphere,
	enCollisionType_AttachableObject,
};


struct CollisionInfo
{
	EnCollisionType m_type;	// 当たり判定の種類(プレイヤーと食材がヒットしたみたいな処理をするために必要。自分がだれかの判断)
	IGameObject* m_object;	// 当たり判定を持つオブジェクトのポインタ
	CollisionObject* m_collision; // 当たり判定オブジェクトのポインタ
	//
	CollisionInfo() : m_type(enCollisionType_Sphere), m_object(nullptr), m_collision(nullptr) {}
	CollisionInfo(const EnCollisionType type, IGameObject* object, CollisionObject* collision) : m_type(type), m_object(object), m_collision(collision) {}
};




struct CollisionPair
{
	CollisionInfo* m_left;	// 当たり判定A
	CollisionInfo* m_right;	// 当たり判定B
	bool isPlayedSE = true;
	//
	CollisionPair() : m_left(nullptr), m_right(nullptr) {}
	CollisionPair(CollisionInfo* left, CollisionInfo* right) : m_left(left), m_right(right) {}
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
	void RegisterCollisionObject(EnCollisionType type, IGameObject* object, CollisionObject* collision);
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
	T* GetTargetObject(CollisionPair& pair, const EnCollisionType targetType)
	{
		if (pair.m_left->m_type == targetType)
		{
			return static_cast<T*>(pair.m_left->m_object);
		}
		else if (pair.m_right->m_type == targetType)
		{
			return static_cast<T*>(pair.m_right->m_object);
		}
		return nullptr;
	}


private:
	static CollisionHitManager* instance_;


public:
	static void Create()
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
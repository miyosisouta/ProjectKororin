#include "stdafx.h"
#include "CollisionManager.h"

#include "Actor/Sphere/Sphere.h"
#include "Actor/Object/AttachableObject.h"
#include "Core/LateStageObjectUpdateManager.h"
#include "Core/InGameManager.h"


CollisionHitManager* CollisionHitManager::instance_ = nullptr;


CollisionHitManager::CollisionHitManager()
{
	m_collisionInfoList.clear();
	m_collisionPairList.clear();
}


CollisionHitManager::~CollisionHitManager()
{
	m_collisionInfoList.clear();
	m_collisionPairList.clear();
}


void CollisionHitManager::Update()
{
	// ヒットするオブジェクトのペアを作る
	const uint32_t colSize = static_cast<uint32_t>(m_collisionInfoList.size());
	for (uint32_t i = 0; i < colSize - 1; ++i) {
		for (uint32_t j = i + 1; j < colSize; ++j) {
			CollisionInfo* infoA = &m_collisionInfoList[i];
			CollisionInfo* infoB = &m_collisionInfoList[j];

			if (infoA->collision->IsHit(infoB->collision) || infoB->collision->IsHit(infoA->collision))
			{
				// CollisionPairの中に同じ組み合わせがないかチェック
				bool exists = false;
				for (const auto& pair : m_collisionPairList) {
					if ((pair.left == infoA && pair.right == infoB) || (pair.left == infoB && pair.right == infoA)) {
						exists = true;
						break;
					}
				}
				// すでに登録済みではないなら追加する
				if (!exists) {
					m_collisionPairList.push_back(CollisionPair(infoA, infoB));
					

				}
			}
		}
	}

	// ヒットしたペアで衝突した時の処理をする
	for (auto& pair : m_collisionPairList)
	{
		// 吸着可能な場合処理を続行
		if (UpdateHitAttatchableObject(pair)) 
		{
			continue;
		}

	}

	m_collisionPairList.clear();
}


//void CollisionHitManager::RegisterCollisionObject(GameObjectType::Enum type, IGameObject* object, CollisionObject* collision)
void CollisionHitManager::RegisterCollisionObject(GameObjectType::Enum type, IGameObject* object, ICollision* collision)
{
	CollisionInfo info(type, object, collision);
	m_collisionInfoList.push_back(std::move(info));
}


void CollisionHitManager::UnregisterCollisionObject(IGameObject* object)
{
	for (auto it = m_collisionInfoList.begin(); it != m_collisionInfoList.end(); ++it)
	{
		if (it->object == object)
		{
			m_collisionInfoList.erase(it);
			break;
		}
	}
}


bool CollisionHitManager::UpdateHitAttatchableObject(CollisionPair& pair)
{
	// pairとして作られた吸着可能なオブジェクトと塊の情報をGetする
	AttachableObject* attachableObject = GetTargetObject<AttachableObject>(pair, GameObjectType::AttachableObject);
	Sphere* sphere = GetTargetObject<Sphere>(pair, GameObjectType::Sphere);

	if (attachableObject == nullptr) {
		return false;
	}
	if (sphere == nullptr) {
		return false;
	}


	/** 吸着可能なオブジェクトとスフィアの当たり判定処理が、ここからできる */


	NotifyCollisionHit* notify = new NotifyCollisionHit();
	notify->left = attachableObject;
	notify->leftType = GameObjectType::AttachableObject;
	notify->right = sphere;
	notify->rightType = GameObjectType::Sphere;

	InGameManager::Get().Notify(notify);
	return true;
}
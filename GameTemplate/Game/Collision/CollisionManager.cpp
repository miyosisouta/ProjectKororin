#include "stdafx.h"
#include "CollisionManager.h"

#include "Actor/Sphere/Sphere.h"
#include "Actor/Object/AttachableObject.h"
#include "Core/LateStageObjectUpdateManager.h"
#include "Core/InGameManager.h"

namespace 
{
	constexpr const float PANEL_WIDTH = 200.0f; // パネルの幅
	constexpr const float PANEL_HEIGHT = 200.0f; // パネルの高さ
}


void SplitSpace::Update()
{
	const auto& collisionInfoList = CollisionHitManager::Get().m_collisionInfoList;
	
	// 全体のサイズ
	const float worldSizeX = worldHalfSize_.x * 2.0f;	
	const float worldSizeZ = worldHalfSize_.z * 2.0f;
	// 1個の空間のサイズ
	const float worldSpaceSizeX = worldSizeX / static_cast<float>(X_NUM);
	const float worldSpaceSizeZ = worldSizeZ / static_cast<float>(Z_NUM);
	// 1個の空間の半分のサイズ
	const float worldSpaceHalfSizeX = worldSpaceSizeX / 2.0f;
	const float worldSpaceHalfSizeZ = worldSpaceSizeZ / 2.0f;
	

	// グリッドの左上（または左下）の基準開始位置
	// ※ AABBの中心座標として扱いやすいように調整
	const float baseX = -worldHalfSize_.x + worldSpaceHalfSizeX;
	const float baseZ = -worldHalfSize_.z + worldSpaceHalfSizeZ;
	for (auto& collisionInfo : collisionInfoList) {
		// オブジェクトごとに判定
		const auto& collisionAABB = collisionInfo.collision->GetAABB();
		for (int x = 0; x < X_NUM; ++x) {
			for (int z = 0; z < Z_NUM; ++z) {

				Vector3 centerPosition;
				centerPosition.x = baseX + (x * worldSpaceSizeX);
				centerPosition.y = 0.0f;
				centerPosition.z = baseZ + (z * worldSpaceSizeZ);
				// 空間のAABB生成
				AABBBox aabb = AABBBox(centerPosition, Vector3(worldSpaceHalfSizeX, 1.0f, worldSpaceHalfSizeZ));
				if (aabb.IsHit(collisionAABB)) {
					colisionSpacialList[x][z].push_back(&collisionInfo);
				}
			}
		}
	}
}


/********************************************************/




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
	// 今回のゲームだと一回だけ更新して空間分割を処理したらいいはず
	// なのでnullptrチェックでnullptrの時生成＋更新で空間に所属するオブジェクトを決める
	if (splitSpace_ == nullptr) {
		splitSpace_ = std::make_unique<SplitSpace>(Vector3(3000.0f, 1.0f, 6000.0f));
		splitSpace_->Update();
	}

	// ヒットするオブジェクトのペアを作る
	const uint32_t colSize = static_cast<uint32_t>(m_collisionInfoList.size());
	for (uint32_t i = 0; i < colSize - 1; ++i) 
	{
		for (uint32_t j = i + 1; j < colSize; ++j) 
		{
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

//void CollisionHitManager::Register()
//{
//}
//
//void CollisionHitManager::RigisterNearObjects()
//{
//}

#include "stdafx.h"
#include "CollisionManager.h"

#include "Actor/Sphere/Sphere.h"
#include "Actor/Object/AttachableObject.h"
#include "Core/LateStageObjectUpdateManager.h"
#include "Core/InGameManager.h"


void SplitSpace::Update()
{
	// collisionHitManagerのオブジェクトの当たり判定が登録されているリストを入手
	auto& collisionInfoList = CollisionHitManager::Get().m_collisionInfoList;

	// オブジェクトと当たり判定を見ていく
	for (auto* collisionInfo : collisionInfoList) {
		// オブジェクトごとの当たり判定を入手
		const auto& collisionAABB = collisionInfo->collision->GetAABB();
		ForEach(collisionAABB, [&](int x, int z)
			{
				colisionSpacialList[x][z].push_back(collisionInfo);
			});
	}

}


void SplitSpace::ForEach(const AABBBox& aabb, const std::function<void(int, int)>& hitFunc)
{
	// 分割した空間の左下から見ていく
	for (int x = 0; x < X_NUM_; ++x)
	{
		for (int z = 0; z < Z_NUM_; ++z)
		{
			// 分割したAABBBoxの中心座標の設定
			Vector3 centerPosition;
			centerPosition.x = baseX_ + (x * worldSpaceSizeX_);
			centerPosition.y = 0.0f;
			centerPosition.z = baseZ_ + (z * worldSpaceSizeZ_);

			// 空間のAABB生成
			AABBBox spaceAABB = AABBBox(centerPosition, Vector3(worldSpaceHalfSizeX_, 1.0f, worldSpaceHalfSizeZ_));
			if (spaceAABB.IsHit(aabb)) {
				hitFunc(x, z);
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
	//// 1回だけ更新するためnullptrチェックを行う
	//if (splitSpace_ == nullptr)
	//{
	//	// 空間分割をするための範囲の生成と設定
	//	splitSpace_ = std::make_unique<SplitSpace>(Vector3(7000.0f, 500.0f, 2500.0f));
	//	// 1度だけ更新
	//	splitSpace_->Update();
	//}

	//// 1.sphereがいる空間の位置をゲット
	//CollisionInfo* sphereInfo = nullptr;
	//for (auto* info : m_collisionInfoList) // 当たり判定のリストからSphereを探す
	//{
	//	if (info->type == GameObjectType::Sphere)
	//	{
	//		// objectがIGameObject型なので型をSpehreに変更
	//		sphereInfo = info;
	//		// 見つかったらループを抜ける
	//		break;
	//	}
	//}
	//if (sphereInfo == nullptr) {
	//	return;
	//}

	//// 2.sphereがいる空間を見つける
	//std::vector<int> xList;
	//std::vector<int> zList;
	//auto* sphere = static_cast<Sphere*>(sphereInfo->object);
	//splitSpace_->ForEach(sphereInfo->collision->GetAABB(), [&](int x, int z)
	//	{
	//		auto xIt = std::find(xList.begin(), xList.end(), x);
	//		if (xIt == xList.end()) {
	//			xList.push_back(x);
	//		}
	//		auto zIt = std::find(zList.begin(), zList.end(), z);
	//		if (zIt == zList.end()) {
	//			zList.push_back(z);
	//		}
	//	});
	//// 3.sphereと空間のオブジェクトが当たっているかどうか判定
	//// 4.ヒットするオブジェクトのペアを作る
	//for (const int x : xList) {
	//	for (const int z : zList) {
	//		const auto& splitSpaceList = splitSpace_->colisionSpacialList[x][z];
	//		for (auto* collisionInfo : splitSpaceList) {
	//			if (sphereInfo->collision->IsHit(collisionInfo->collision)) {
	//				// CollisionPairの中に同じ組み合わせがないかチェック
	//				bool exists = false;
	//				for (const auto& pair : m_collisionPairList) {
	//					if ((pair->left == sphereInfo && pair->right == collisionInfo) || (pair->left == collisionInfo && pair->right == sphereInfo)) {
	//						exists = true;
	//						break;
	//					}
	//				}
	//				if (!exists) {
	//					m_collisionPairList.push_back(std::make_unique<CollisionPair>(sphereInfo, collisionInfo));
	//				}
	//			}
	//		}
	//	}
	//}

	const uint32_t colSize = static_cast<uint32_t>(m_collisionInfoList.size()); // 当たり判定を持つ全オブジェクトのリストを入手

	for (uint32_t i = 0; i < colSize - 1; ++i)  // リストの中にあるデータの最後から2番目までをはかる : 最後の一つはペアを作れないため
	{
		for (uint32_t j = i + 1; j < colSize; ++j)
		{
			// オブジェクト情報を格納
			CollisionInfo* infoA = m_collisionInfoList[i];
			CollisionInfo* infoB = m_collisionInfoList[j];

			// 当たった場合
			if (infoA->collision->IsHit(infoB->collision) || infoB->collision->IsHit(infoA->collision))
			{
				// CollisionPairの中に同じ組み合わせがないかチェック
				bool exists = false;
				for (const auto& pair : m_collisionPairList) {
					if ((pair->left == infoA && pair->right == infoB) || (pair->left == infoB && pair->right == infoA)) {
						exists = true;
						break;
					}
				}
				// すでに登録済みではないなら追加する
				if (!exists) {
					m_collisionPairList.push_back(std::make_unique<CollisionPair>(infoA, infoB));
				}
			}
		}
	}

	// ヒットしたペアで衝突した時の処理をする
	for (auto& pair : m_collisionPairList)
	{
		// 吸着可能な場合処理を続行
		if (UpdateHitAttatchableObject(*pair.get()))
		{
			continue;
		}

	}

	m_collisionPairList.clear();
}


//void CollisionHitManager::RegisterCollisionObject(GameObjectType::Enum type, IGameObject* object, CollisionObject* collision)
void CollisionHitManager::RegisterCollisionObject(GameObjectType::Enum type, IGameObject* object, ICollision* collision)
{
	CollisionInfo* info = new CollisionInfo(type, object, collision);
	m_collisionInfoList.push_back(info);
}


void CollisionHitManager::UnregisterCollisionObject(IGameObject* object)
{
	std::vector<CollisionInfo*> deleteList;
	for (auto it = m_collisionInfoList.begin(); it != m_collisionInfoList.end(); ++it)
	{
		if ((*it)->object == object)
		{
			// 空間分割からも消す
			if (splitSpace_) {
				splitSpace_->ForEach((*it)->collision->GetAABB(), [&](int x, int z)
					{
						auto& list = splitSpace_->colisionSpacialList[x][z];
						auto* collisionInfo = *it;
						auto itr = std::find(list.begin(), list.end(), collisionInfo);
						if (itr != list.end()) {
							list.erase(itr);
						}
					});
			}
			deleteList.push_back(*it);
			m_collisionInfoList.erase(it);
			break;
		}
	}
	for (auto* target : deleteList) {
		delete target;
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
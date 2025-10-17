#include "stdafx.h"
#include "LateStageObjectUpdateManager.h"
#include "Actor/Sphere/Sphere.h"
#include "Actor/Object/StageObjectBase.h"
#include "Actor/Object/AttachableObject.h"


LateStageObjectUpdateManager* LateStageObjectUpdateManager::instance_ = nullptr;


LateStageObjectUpdateManager::LateStageObjectUpdateManager()
{
	objectList_.clear();
}


LateStageObjectUpdateManager::~LateStageObjectUpdateManager()
{
	objectList_.clear();
}


void LateStageObjectUpdateManager::Update()
{
	// @todo for test
	return;

	//sphere_->GetCollider()
	if (sphere_ != nullptr) {
		const auto& sphereCollision = sphere_->GetCollisionObject()->GetbtCollisionObject();
		auto* shpereBody = sphere_->GetCollider()->GetBody();
		const int size = shpereBody->getNumChildShapes(); // 塊の当たり判定の子の数を取得
		if (size > 0) {
			const auto* childList = shpereBody->getChildList();
			// 塊に吸着しているオブジェクトの数だけ繰り返す
			for (int i = 0; i < size; ++i)
			{
				const auto& child = childList[i];
				const auto& childTransform = child.m_transform;
				auto& origin = childTransform.getOrigin();
				K2_LOG("Position: X: %f, Y: %f, Z: %f \n", origin.x(), origin.y(), origin.z());

				auto parentTransform = sphereCollision.getWorldTransform();

				const auto computeTransform = parentTransform * childTransform;
				
				auto* targetObject = FindTargetObject(child.m_childShape);
				if (targetObject) {
					// @todo for test
					auto* targetTransform = static_cast<AttachableObject*>(targetObject)->GetTransform();
					targetTransform->m_localPosition = Vector3(computeTransform.getOrigin().x(), computeTransform.getOrigin().y(), computeTransform.getOrigin().z());
				}
			}
		}
	}


	//// ヒットするオブジェクトのペアを作る
	//const uint32_t colSize = static_cast<uint32_t>(m_collisionInfoList.size());
	//for (uint32_t i = 0; i < colSize - 1; ++i) {
	//	for (uint32_t j = i + 1; j < colSize; ++j) {
	//		CollisionInfo* infoA = &m_collisionInfoList[i];
	//		CollisionInfo* infoB = &m_collisionInfoList[j];

	//		if (infoA->m_collision->IsHit(infoB->m_collision) || infoB->m_collision->IsHit(infoA->m_collision))
	//		{
	//			// CollisionPairの中に同じ組み合わせがないかチェック
	//			bool exists = false;
	//			for (const auto& pair : m_collisionPairList) {
	//				if ((pair.m_left == infoA && pair.m_right == infoB) || (pair.m_left == infoB && pair.m_right == infoA)) {
	//					exists = true;
	//					break;
	//				}
	//			}
	//			// すでに登録済みではないなら追加する
	//			if (!exists) {
	//				m_collisionPairList.push_back(CollisionPair(infoA, infoB));
	//				

	//			}
	//		}
	//	}
	//}

	//// ヒットしたペアで衝突した時の処理をする
	//for (auto& pair : m_collisionPairList)
	//{
	//	// 吸着可能な場合処理を続行
	//	if (UpdateHitAttatchableObject(pair)) 
	//	{
	//		continue;
	//	}

	//}

	//m_collisionPairList.clear();
}


void LateStageObjectUpdateManager::RegisterObject(StageObjectBase* object)
{
	objectList_.push_back(object);
}


void LateStageObjectUpdateManager::UnregisterObject(StageObjectBase* object)
{
	for (auto it = objectList_.begin(); it != objectList_.end(); ++it)
	{
		if (*it == object)
		{
			objectList_.erase(it);
			break;
		}
	}
}


StageObjectBase* LateStageObjectUpdateManager::FindTargetObject(btCollisionShape* target)
{
	for (auto* object : objectList_) {
		if (target = object->GetPhysicsStaticObject()->GetCollider()->GetBody()) {
			return object;
		}
	}
	return nullptr;
}
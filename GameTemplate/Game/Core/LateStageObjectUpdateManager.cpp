#include "stdafx.h"
#include "LateStageObjectUpdateManager.h"
#include "Actor/Sphere/Sphere.h"
#include "Actor/Object/StageObjectBase.h"
#include "Actor/Object/AttachableObject.h"

// インスタンス
LateStageObjectUpdateManager* LateStageObjectUpdateManager::instance_ = nullptr;


LateStageObjectUpdateManager::LateStageObjectUpdateManager()
{
	// リストの削除
	objectList_.clear();
}


LateStageObjectUpdateManager::~LateStageObjectUpdateManager()
{
	// リストの削除
	objectList_.clear();
}


void LateStageObjectUpdateManager::Update()
{
	// @todo for test
	return;

	//sphere_->GetCollider()
	//if (sphere_ != nullptr) {
	//	const auto& sphereCollision = sphere_->GetCollisionObject()->GetbtCollisionObject(); // 衝突判定のコリジョンをゲット
	//	auto* shpereBody = sphere_->GetCollider()->GetBody(); // 当たり判定をゲット
	//	const int size = shpereBody->getNumChildShapes(); // 塊の当たり判定の子の数を取得
	//	if (size > 0) {
	//		const auto* childList = shpereBody->getChildList(); // 塊の当たり判定にあたったオブジェクトのリストを取得

	//		// 塊に吸着しているオブジェクトの数だけ繰り返す
	//		for (int i = 0; i < size; ++i)
	//		{
	//			const auto& child = childList[i]; // 子になったオブジェクトのリストを取得
	//			const auto& childTransform = child.m_transform; // 子のローカル座標を取得

	//			auto& origin = childTransform.getOrigin(); // 座標確認用変数
	//			K2_LOG("Position: X: %f, Y: %f, Z: %f \n", origin.x(), origin.y(), origin.z());

	//			auto parentTransform = sphereCollision.getWorldTransform(); // 塊のワールド座標を取得
	//			const auto computeTransform = parentTransform * childTransform; // 子のワールド座標を取得
	//			
	//			auto* targetObject = FindTargetObject(child.m_childShape); // 子の当たり判定を探す

	//			// 子の当たり判定があった場合
	//			if (targetObject) {
	//				// @todo for test
	//				auto* targetTransform = static_cast<AttachableObject*>(targetObject)->GetTransform(); // AttachableObject型のトランスフォームを取得
	//				targetTransform->m_localPosition = Vector3(computeTransform.getOrigin().x(), computeTransform.getOrigin().y(), computeTransform.getOrigin().z()); // 子のローカル座標にワールド座標を代入
	//			}
	//		}
	//	}
	//}


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
	// データをリストに登録
	objectList_.push_back(object);
}


void LateStageObjectUpdateManager::UnregisterObject(StageObjectBase* object)
{
	// 特定の情報を削除
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
	// 剛体の情報を返す
	for (auto* object : objectList_) {
		if (target = object->GetPhysicsStaticObject()->GetCollider()->GetBody()) {
			return object;
		}
	}
	return nullptr;
}
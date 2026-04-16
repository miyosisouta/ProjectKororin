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
	return;
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
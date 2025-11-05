#include "stdafx.h"
#include "InGameManager.h"
#include "Actor/Sphere/Sphere.h"
#include "Actor/Object/AttachableObject.h"
#include "Core/GameUIManager.h"
#include "Sound/SoundManager.h"


void InGameUpdateObject::Update()
{
	InGameManager::Get().Update();
}


void InGameLateUpdateObject::Update()
{
	InGameManager::Get().LateUpdate();
}



/*********************************/


InGameManager* InGameManager::instance_ = nullptr;


InGameManager::InGameManager()
{
	notifyList_.clear();
}


InGameManager::~InGameManager()
{
	ClearNotify();
}


void InGameManager::Update()
{

}


void ::InGameManager::LateUpdate()
{
	// 通知が入ったら実行される
	for (auto& notify : notifyList_) { // Listの中から通知を一つずつ実行
		switch (notify->type) 
		{
			case NotifyType::CollisionHit: // 通知の種類がCollisionHitだった場合
			{
				NotifyCollisionHit* hitNotify = static_cast<NotifyCollisionHit*>(notify);

				Sphere* sphere = GetTargetObject<Sphere>(*hitNotify, GameObjectType::Sphere);
				AttachableObject* attachableObject = GetTargetObject<AttachableObject>(*hitNotify, GameObjectType::AttachableObject);


				// コリジョン同士が当たった時の処理
				// Sphereのレベルが吸着可能レベルに達しているか判定
				if (!CanAttach(*attachableObject, *sphere))
				{
					continue;
				}

				if (attachableObject->IsPlayedSE()) {
					continue;
				}


				// pairが引っ付いたとき一度だけSEを流す
				if (!attachableObject->IsPlayedSE())  
				{
					SoundManager::Get().PlaySE(enSoundKind_AttachSuccess);
					attachableObject->SetPlayedSE(true);
				}


				// くっついた！
				sphere->AddCurrentLevelUpNum();
				//オブジェクトを塊につけ、一緒に動くようにする
				sphere->SetParent(attachableObject); // transformの親子関係を設定
				attachableObject->GetTransform()->ResetLocalPosition(); // ローカルポジションの初期化
				attachableObject->GetTransform()->ResetLocalRotation(); // ローカルローテーションの初期化
				attachableObject->DeletePhysicsStatics();


				// 塊の半径を広げる
				sphere->GrowByRadius(attachableObject->GetGrowAmount());


				/***************** ここから先は通知についての処理 ********************/


				// 飛ばしたい情報を持つ構造体の型を作成
				NotifySphereSizeText* sphereSizeNotify = new NotifySphereSizeText; // UI/SphereSizeTextに情報を渡すもの
				NotifyObjectView* objectViewNotify = new NotifyObjectView; // UI/ObjectViewに情報を渡すもの

				// sphereSizeについての情報と通知を送信
				sphereSizeNotify->radius = sphere->GetRadius();
				GameUIManager::Get().Notify(sphereSizeNotify);

				// ObjectViewについての情報と通知を送信
				GameUIManager::Get().Notify(objectViewNotify);
				break;
			}

			default: // それ以外の場合通知がない
			{
				K2_ASSERT(false, "通知の種類が追加されていません。\n");
				break;
			}
		}
	}

	ClearNotify();
}


void InGameManager::Notify(INotify* notify)
{
	// 通知を入れる
	notifyList_.push_back(notify);
}

bool InGameManager::CanAttach(AttachableObject& target, Sphere& sphere)
{
	// 吸着可能オブジェクトかどうかの判定
	if (target.GetObjectSize() == 0)
	{
		return true;
	}

	// 塊のレベルがオブジェクトのサイズ以上の場合
	if (sphere.GetStatus()->GetLevel() >= target.GetObjectSize()) {
		return true;;
	}

	return false;
}
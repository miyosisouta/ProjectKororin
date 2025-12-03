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

	// インゲーム固有のパラメーターを読み込みます
	ParameterManager::Get().LoadParameter<MasterInGameParameter>("Assets/Parameter/InGameParameter.json", [](const nlohmann::json& json, MasterInGameParameter& p)
		{
			p.BouncePower[0] = json["BouncePower1"].get<float>();
			p.BouncePower[1] = json["BouncePower2"].get<float>();
			p.BouncePower[2] = json["BouncePower3"].get<float>();
			p.BouncePower[3] = json["BouncePower4"].get<float>();
			p.BouncePower[4] = json["BouncePowerMax"].get<float>();
			p.limitTime = json["LimitTime"].get<float>();
		});
}


InGameManager::~InGameManager()
{
	ClearNotify();
	// パラメーター破棄
	ParameterManager::Get().UnloadParameter<MasterInGameParameter>();
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
					// レベルに達していないので飛ばす
					Vector3 vec = sphere->GetTransform()->m_position - attachableObject->GetTransform()->m_position;
					vec.y = 0.0f;
					vec.Normalize();

					// TODO:あとではじけ飛ぶ数値を調整可能にする
					float bouncePower = 0.0f;
					{
						const auto* param = ParameterManager::Get().GetParameter<MasterInGameParameter>();
						int objectSize = attachableObject->GetObjectSize();
						if (objectSize >= ARRAYSIZE(param->BouncePower)) {
							objectSize = ARRAYSIZE(param->BouncePower) - 1;
						}
						bouncePower = param->BouncePower[objectSize];
					}
					sphere->AddForce(vec * bouncePower);

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


				sphere->AddCurrentLevelUpNum(); // 引っ付いたオブジェクトの数を増加
				sphere->AddTotalNum(); // 引っ付いたオブジェクトの合計の数を増加
				sphere->GrowByRadius(attachableObject->GetGrowAmount()); // オブジェクトの半径を増加・移動速度の制限


				//オブジェクトを塊につけ、一緒に動くようにする
				sphere->SetParent(attachableObject); // transformの親子関係を設定
				attachableObject->DeletePhysicsStatics();


				// オブジェクトがどのあたりにくっつくかを処理してみる
				Vector3 direction = attachableObject->GetTransform()->m_position - sphere->GetTransform()->m_position;
				direction.Normalize();

				// 現在の半径を取得
				const float sphereSize = sphere->GetEffectiveRadius();

				// 塊のワールド行列を取得・逆行列にする
				Matrix sphereWorldMatrix = sphere->GetTransform()->GetWorldMatrix(); 
				sphereWorldMatrix.Inverse();

				// トランスフォームの取得
				const Transform* attachableObjectTransform = attachableObject->GetTransform();

				Matrix mWorld; // 吸着オブジェクトがワールド空間内のどこにあるかを示す行列
				Matrix mTrans, mRot, mScale; // 行列の変数を定義
				mTrans.MakeTranslation(attachableObjectTransform->m_position); // 平行移動行列の作成
				mRot.MakeRotationFromQuaternion(attachableObjectTransform->m_rotation); // 回転行列の作成
				mScale.MakeScaling(attachableObjectTransform->m_scale); // 拡大行列を取得
				mWorld = mScale * mRot * mTrans;

				Matrix computeMatrix = sphereWorldMatrix * mWorld; // 塊のローカル行列を取得

				Vector3& localPos = attachableObject->GetTransform()->m_localPosition; // オブジェクトのローカル座標を取得
				Quaternion& localRot = attachableObject->GetTransform()->m_localRotation; // オブジェクトのローカル回転を取得
				localPos.x = computeMatrix.m[3][0] * 1.0f;
				localPos.y = computeMatrix.m[3][1] * 1.0f;
				localPos.z = computeMatrix.m[3][2] * 1.0f;

				localPos.Normalize();
				localPos.Scale(sphereSize);

				localRot.SetRotation(computeMatrix);

				/***************** ここから先は吸着時に飛ばす通知についての処理 ********************/


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
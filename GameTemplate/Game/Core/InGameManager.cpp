#include "stdafx.h"
#include "InGameManager.h"
#include "Actor/Sphere/Sphere.h"
#include "Actor/Object/AttachableObject.h"
#include "Core/GameUIManager.h"
#include "Sound/SoundManager.h"


void InGameUpdateObject::Update()
{
	// 更新処理
	InGameManager::Get().Update();
}


void InGameLateUpdateObject::Update()
{
	// 遅れて更新処理
	InGameManager::Get().LateUpdate();
}



/*********************************/

// インスタンス
InGameManager* InGameManager::instance_ = nullptr;


InGameManager::InGameManager()
{
	// リストの要素を削除
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
	// リストを削除
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

			// 情報を取得
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


				// ノックバックの処理
				float bouncePower = 0.0f;
				{
					// はじけとぶ強さ
					const auto* param = ParameterManager::Get().GetParameter<MasterInGameParameter>();
					int objectSize = attachableObject->GetObjectSize();
					const int bouncePowerSize = ARRAYSIZE(param->BouncePower);
					if (objectSize >= bouncePowerSize) {
						objectSize = bouncePowerSize - 1;
					}
					bouncePower = param->BouncePower[objectSize];

					// コントローラー振動
					const float vibrationPower = objectSize / static_cast<float>(bouncePowerSize);				// 強さ
					const float vibrationDurationSec = objectSize / static_cast<float>(bouncePowerSize) * 0.8f;	// 長さ
					g_pad[0]->SetVibration(vibrationDurationSec, vibrationPower);
				}
				sphere->AddForce(vec * bouncePower); // 加える力の計算

				continue;
			}

			// SEをすでに鳴らしている場合処理を返す
			if (attachableObject->IsPlayedSE()) {
				continue;
			}


			// pairが引っ付いたとき一度だけSEを流す
			if (!attachableObject->IsPlayedSE())
			{
				switch (attachableObject->GetAttachSoundNum())
				{
				case 0:
					SoundManager::Get().PlaySE(enSoundKind_Attach_Tiny);
					break;
				case 1:
					SoundManager::Get().PlaySE(enSoundKind_Attach_Small);
					break;
				case 2:
					SoundManager::Get().PlaySE(enSoundKind_Attach_Medium);
					break;
				case 3:
					SoundManager::Get().PlaySE(enSoundKind_Attach_Large);
					break;
				default:
					SoundManager::Get().PlaySE(enSoundKind_AttachSuccess);
					break;
				}
				attachableObject->SetPlayedSE(true);
			}

			// 引っ付いたときの塊の処理

			sphere->AddCurrentLevelUpNum();							// 引っ付いたオブジェクトの数を増加
			sphere->AddTotalNum();									// 引っ付いたオブジェクトの合計の数を増加
			sphere->GrowByRadius(attachableObject->GetGrowAmount());// オブジェクトの半径を増加・移動速度の制限


			//オブジェクトを塊につけ、一緒に動くようにする
			sphere->SetParent(attachableObject);				// transformの親子関係を設定
			sphere->AddAttachableObject(attachableObject);		// Listに追加
			attachableObject->SetVisibleAttachedObject(true);	// 吸着したオブジェクトを描画可能とする
			attachableObject->DeletePhysicsStatics();			// 当たり判定の破棄


			// 現在の半径を取得
			const float sphereSize = sphere->GetEffectiveRadius();

			// 塊のワールド行列を取得・逆行列にする
			Matrix sphereWorldMatrix = sphere->GetTransform()->GetWorldMatrix();
			sphereWorldMatrix.Inverse();

			// トランスフォームの取得
			const Transform* attachableObjectTransform = attachableObject->GetTransform();

			Matrix mWorld;					// 吸着オブジェクトがワールド空間内のどこにあるかを示す行列
			Matrix mTrans, mRot, mScale;	// 行列の変数を定義
			mTrans.MakeTranslation(attachableObjectTransform->m_position);			// 平行移動行列の作成
			mRot.MakeRotationFromQuaternion(attachableObjectTransform->m_rotation); // 回転行列の作成
			mScale.MakeScaling(attachableObjectTransform->m_scale);					// 拡大行列を取得
			mWorld = mScale * mRot * mTrans; // オブジェクトのワールド行列を取得

			Matrix computeMatrix = sphereWorldMatrix * mWorld; // 塊のローカル行列を取得

			Vector3& localPos = attachableObject->GetTransform()->m_localPosition;		// オブジェクトのローカル座標を取得
			Quaternion& localRot = attachableObject->GetTransform()->m_localRotation;	// オブジェクトのローカル回転を取得
			localPos.x = computeMatrix.m[3][0] * 1.0f;
			localPos.y = computeMatrix.m[3][1] * 1.0f;
			localPos.z = computeMatrix.m[3][2] * 1.0f;

			localPos.Normalize();					// 正規化
			localPos.Scale(sphereSize);				// 大きさを設定
			localRot.SetRotation(computeMatrix);	// 回転の設定

			// オフスクリーンレンダリングに設定
			g_renderingEngine->SetModel(attachableObject->GetModelRender(), attachableObject->GetUIScale());

			/***************** ここから先は吸着時に飛ばす通知についての処理 ********************/


			// 飛ばしたい情報を持つ構造体の型を作成
			NotifySphereSizeText* sphereSizeNotify = new NotifySphereSizeText; // UI/SphereSizeTextに情報を渡すもの

			// sphereSizeについての情報と通知を送信
			sphereSizeNotify->radius = sphere->GetRadius(); // 半径を渡す
			GameUIManager::Get().Notify(sphereSizeNotify);	// 通知を飛ばす
			break;
		}

		default: // それ以外の場合通知がない
		{
			K2_ASSERT(false, "通知の種類が追加されていません。\n");
			break;
		}
		}
	}

	// リストの削除
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
#include "stdafx.h"
#include "StageObjectBase.h"
#include "Actor/Sphere/Sphere.h"
#include "Collision/Collision.h"


namespace
{
	const float COLLISION_POSITIONADJUSTMENT = 35.0f;	// コリジョンの起点の座標を調整する
	const float COLLISION_CORRECTION = 20.0f;			// コリジョンの大きさを調整する
}


bool StageObjectBase::Start()
{
	// モデルの初期化
	// アセットの名前は文字列の連結をしないといけない
	std::string modelName = "Assets/modelData/stage/" + assetName_ + ".tkm";
	modelRender_.Init(modelName.c_str());

	// transformの更新
	transform_.UpdateTransform();


	// 当たり判定の設定
	{
		// コライダーの設定
		Vector3 collisionCorrection = ApplyCollisionSizeDelta(transform_.m_localScale) * COLLISION_CORRECTION; // スケールの調整
		position_ = transform_.m_position + colliderPivot_ * COLLISION_POSITIONADJUSTMENT;	// 起点
		boxColliderScale_ = Multply(collisionCorrection, colliderSize_); // サイズ

		//当たり判定に実態を持たせる
		physicsStaticObject_ = new PhysicsStaticObject;
		// boxcolliderの当たり判定を作成
		physicsStaticObject_->CreateBoxCollider(position_, boxColliderScale_, modelRender_.GetModel().GetWorldMatrix());
	}


	// モデルの設定
	modelRender_.SetPosition(transform_.m_position);
	modelRender_.SetScale(transform_.m_scale);
	modelRender_.SetRotation(transform_.m_rotation);
	modelRender_.Update();

	// transformの再更新
	transform_.UpdateTransform();
	return true;
}


void StageObjectBase::Initialize(ObjectData* objectData)
{
	// トランスフォームの設定
	{
		transform_.m_localPosition = objectData->position;	// 座標
		transform_.m_localScale = objectData->scale;		// 大きさ
		transform_.m_localRotation = objectData->rotation;	// 回転
	}

	// 固有パラメーターの設定
	{
		attachableValue_ = objectData->attachValue; // 吸着可能かどうか
		objectSize_ = objectData->size;				// オブジェクトの吸着可能サイズ
		assetName_ = objectData->assetName;			// アセットの名前
		grouthAmount_ = objectData->grouthAmount;	// 塊のサイズの増加量
		colliderPivot_ = objectData->colliderPivot; // コライダーの起点の座標
		colliderSize_ = objectData->colliderSize;	// コライダーの大きさ
		UIDisplayscale_ = objectData->uiObjectScal; // UIとして表示する際のオブジェクトの大きさの倍率
		soundNum_ = objectData->soundNum;			// 吸着時のSEの番号設定
		ID_ = objectData->ID;
	}
}

Vector3 StageObjectBase::ApplyCollisionSizeDelta(Vector3 scale)
{
	Vector3 adjusted_size = Vector3(scale.x - 1.0f, scale.y - 1.0f, scale.z - 1.0f);

	// xの差異を調整
	if (adjusted_size.x <= 0.0f) { adjusted_size.x = 1.0f; }

	// yの差異を調整
	if (adjusted_size.y <= 0.0f) { adjusted_size.y = 1.0f; }

	// zの差異を調整
	if (adjusted_size.z <= 0.0f) { adjusted_size.z = 1.0f; }

	return adjusted_size;
}

Vector3 StageObjectBase::Multply(Vector3 a, Vector3 b)
{
	return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

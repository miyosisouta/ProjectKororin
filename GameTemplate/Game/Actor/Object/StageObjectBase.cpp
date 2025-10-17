#include "stdafx.h"
#include "StageObjectBase.h"
#include "Actor/Sphere/Sphere.h"


namespace 
{
	const float COLLISION_POSITIONADJUSTMENT = 35.0f;	// コリジョンの起点の座標を調整する
	const float COLLISION_CORRECTION = 40.0f;			// コリジョンの大きさを調整する
}
bool StageObjectBase:: Start() 
{
	// モデルの初期化
	// アセットの名前は文字列の連結をしないといけない
	std::string modelName = "Assets/modelData/stage/" + assetName_ + ".tkm";
	modelRender_.Init(modelName.c_str());
	
	// transformのアップデート
	transform_.UpdateTransform();


	// コライダーの変更
	Vector3 collisionCorrection = ApplyCollisionSizeDelta(transform_.m_localScale) * COLLISION_CORRECTION; // スケールの調整
	Vector3 colliderPos = transform_.m_position + colliderPivot_* COLLISION_POSITIONADJUSTMENT;	// 起点
	Vector3 colliderSize = Multply(collisionCorrection, colliderSize_);							// サイズ
	//当たり判定に実態を持たせる
	physicsStaticObject_ = new PhysicsStaticObject;
	// boxcolliderの当たり判定
	physicsStaticObject_->CreateBoxCollider(colliderPos, colliderSize, modelRender_.GetModel().GetWorldMatrix());
	// todo for test : 当たり判定の描画
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	collisionObject_ = NewGO<CollisionObject>(0);
	collisionObject_->CreateBox(colliderPos, transform_.m_rotation, colliderSize);
	collisionObject_->SetIsEnableAutoDelete(false);


	// モデルの設定
	modelRender_.SetPosition(transform_.m_position);
	modelRender_.SetScale(transform_.m_scale);
	modelRender_.SetRotation(transform_.m_rotation);
	modelRender_.Update();
	
	// transformのアップデート
	transform_.UpdateTransform();
	return true;
}


Vector3 StageObjectBase::ApplyCollisionSizeDelta(Vector3 scale)
{
	Vector3 adjusted_size = Vector3(scale.x - 1.0f, scale.y - 1.0f, scale.z - 1.0f);

	if (adjusted_size.x <= 0.0f)
	{
		adjusted_size.x = 1.0f;
	}

	if (adjusted_size.y <= 0.0f)
	{
		adjusted_size.y = 1.0f;
	}
	
	if (adjusted_size.z <= 0.0f)
	{
		adjusted_size.z = 1.0f;
	}


	return adjusted_size;
}

Vector3 StageObjectBase::Multply(Vector3 a, Vector3 b)
{
	return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

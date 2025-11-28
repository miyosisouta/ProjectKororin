#include "stdafx.h"
#include "Stage.h"
#include "Collision/CollisionManager.h"

AttachableObject::~AttachableObject()
{
	if (physicsStaticObject_)
	{
		physicsStaticObject_->Release(); // 剛体(当たり判定)を破棄
		delete physicsStaticObject_;
		physicsStaticObject_ = nullptr;
	}

	CollisionHitManager::Get().UnregisterCollisionObject(this);
	if (collisionObject_) {
		DeleteGO(collisionObject_);
		collisionObject_ = nullptr;
	}
}

bool AttachableObject::Start()
{
	StageObjectBase::Start();

	// コリジョンの設定
	// BoxCollisionのユニークポインタの作成と初期化
	auto boxCollider = std::make_unique<BoxCollision>();
	boxCollider->Init(GetPosition(), GetSize());

	// ICollisionに所有権を移動
	collider_ = std::move(boxCollider);
	// 当たり判定を登録
	CollisionHitManager::Get().RegisterCollisionObject(
		GameObjectType::AttachableObject,   // 種類（オブジェクトの場合）
		this,								// ひっつくオブジェクト自身
		collider_.get()						// そのコリジョン
	);

	return true;
}

void AttachableObject::Update() 
{
	//// @todo for test
	//btRigidBody* btBody = physicsStaticObject_->GetRigidBody()->GetBody(); // 当たり判定の剛体を取得
	//btTransform& trans = btBody->getWorldTransform(); // 剛体のワールド座標を取得

	//physicsStaticObject_->GetCollider()->GetBody(); 

	//// transに入っているワールド座標をローカル座標に格納
	//transform_.m_localPosition = Vector3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z()); 


	transform_.UpdateTransform();
	modelRender_.SetPosition(transform_.m_position);
	modelRender_.SetRotation(transform_.m_rotation);
	modelRender_.Update();
}

void AttachableObject::Render(RenderContext& rc) 
{
	if (isVisible_) {
		modelRender_.Draw(rc);
	}
}


void AttachableObject::DeletePhysicsStatics()
{
	if (physicsStaticObject_)
	{
		physicsStaticObject_->Release(); // 剛体(当たり判定)を破棄
		delete physicsStaticObject_;
		physicsStaticObject_ = nullptr;
	}

	CollisionHitManager::Get().UnregisterCollisionObject(this);
	if (collisionObject_) 
	{
		DeleteGO(collisionObject_);
		collisionObject_ = nullptr;
	}
}

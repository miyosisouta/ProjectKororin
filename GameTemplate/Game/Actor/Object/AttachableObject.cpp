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
}

bool AttachableObject::Start()
{
	StageObjectBase::Start();

	// 当たり判定を登録
	CollisionHitManager::Get().RegisterCollisionObject(
		enCollisionType_AttachableObject,   // 種類（オブジェクトの場合）
		this,								// ひっつくオブジェクト自身
		collisionObject_					// そのコリジョン
	);

	return true;
}

void AttachableObject::Update() 
{
	btRigidBody* btBody = physicsStaticObject_->GetRigidBody()->GetBody();
	btTransform& trans = btBody->getWorldTransform();

	physicsStaticObject_->GetCollider()->GetBody()

	transform_.m_localPosition = Vector3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());

	transform_.UpdateTransform();
	modelRender_.SetPosition(transform_.m_position);
	modelRender_.SetRotation(transform_.m_rotation);
	modelRender_.Update();
}

void AttachableObject::Render(RenderContext& rc) 
{
	modelRender_.Draw(rc);
}

void AttachableObject::DeletePhysicsStatics()
{
	if (physicsStaticObject_)
	{
		physicsStaticObject_->Release(); // 剛体(当たり判定)を破棄
		delete physicsStaticObject_;
		physicsStaticObject_ = nullptr;
	}
}

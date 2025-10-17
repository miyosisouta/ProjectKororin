#include "stdafx.h"
#include "Stage.h"
#include "Collision/CollisionManager.h"

AttachableObject::~AttachableObject()
{
	if (physicsStaticObject_)
	{
		physicsStaticObject_->Release(); // ����(�����蔻��)��j��
		delete physicsStaticObject_;
		physicsStaticObject_ = nullptr;
	}
}

bool AttachableObject::Start()
{
	StageObjectBase::Start();

	// �����蔻���o�^
	CollisionHitManager::Get().RegisterCollisionObject(
		enCollisionType_AttachableObject,   // ��ށi�I�u�W�F�N�g�̏ꍇ�j
		this,								// �Ђ����I�u�W�F�N�g���g
		collisionObject_					// ���̃R���W����
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
		physicsStaticObject_->Release(); // ����(�����蔻��)��j��
		delete physicsStaticObject_;
		physicsStaticObject_ = nullptr;
	}
}

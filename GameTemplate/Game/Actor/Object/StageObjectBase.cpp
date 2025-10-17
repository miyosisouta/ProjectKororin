#include "stdafx.h"
#include "StageObjectBase.h"
#include "Actor/Sphere/Sphere.h"


namespace 
{
	const float COLLISION_POSITIONADJUSTMENT = 35.0f;	// �R���W�����̋N�_�̍��W�𒲐�����
	const float COLLISION_CORRECTION = 40.0f;			// �R���W�����̑傫���𒲐�����
}
bool StageObjectBase:: Start() 
{
	// ���f���̏�����
	// �A�Z�b�g�̖��O�͕�����̘A�������Ȃ��Ƃ����Ȃ�
	std::string modelName = "Assets/modelData/stage/" + assetName_ + ".tkm";
	modelRender_.Init(modelName.c_str());
	
	// transform�̃A�b�v�f�[�g
	transform_.UpdateTransform();


	// �R���C�_�[�̕ύX
	Vector3 collisionCorrection = ApplyCollisionSizeDelta(transform_.m_localScale) * COLLISION_CORRECTION; // �X�P�[���̒���
	Vector3 colliderPos = transform_.m_position + colliderPivot_* COLLISION_POSITIONADJUSTMENT;	// �N�_
	Vector3 colliderSize = Multply(collisionCorrection, colliderSize_);							// �T�C�Y
	//�����蔻��Ɏ��Ԃ���������
	physicsStaticObject_ = new PhysicsStaticObject;
	// boxcollider�̓����蔻��
	physicsStaticObject_->CreateBoxCollider(colliderPos, colliderSize, modelRender_.GetModel().GetWorldMatrix());
	// todo for test : �����蔻��̕`��
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	collisionObject_ = NewGO<CollisionObject>(0);
	collisionObject_->CreateBox(colliderPos, transform_.m_rotation, colliderSize);
	collisionObject_->SetIsEnableAutoDelete(false);


	// ���f���̐ݒ�
	modelRender_.SetPosition(transform_.m_position);
	modelRender_.SetScale(transform_.m_scale);
	modelRender_.SetRotation(transform_.m_rotation);
	modelRender_.Update();
	
	// transform�̃A�b�v�f�[�g
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

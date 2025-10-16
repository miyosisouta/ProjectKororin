#include "stdafx.h"
#include "Sphere.h"
#include <iostream>
#include "Collision/CollisionManager.h"
#include "Actor/Object/AttachableObject.h"
namespace 
{
	const float ALWAYS_SPEED = 100.0f;	// �Œ�ړ����x
	const float INITIAL_RADIUS = 15.0f;	// �������a
	//const float INITIAL_MASS = 1.0f; // ��������
}

bool Sphere::Start() 
{
	// ���f����Init
	sphereRender_.Init("Assets/modelData/mass/model/mass.tkm");

	// ������
	transform_.m_scale = Vector3(radius_, radius_, radius_);
	sphereRender_.SetPosition(transform_.m_position);
	sphereRender_.SetRotation(transform_.m_rotation);
	sphereRender_.SetScale(transform_.m_scale);
	sphereRender_.Update();


	// �R���W�������쐬
	collisionObject_ = NewGO<CollisionObject>(0, "collisionObject");
	collisionObject_->CreateSphere(
		transform_.m_position,	// ���W
		transform_.m_rotation,	// ��]
		INITIAL_RADIUS			// ���`�̔��a
	);
	// �R���W�����������Ȃ��悤�ɂ���
	collisionObject_->SetIsEnableAutoDelete(false);


	// �����蔻��̓o�^
	CollisionHitManager::Get().RegisterCollisionObject(
		enCollisionType_Sphere,   // ���
		this,                     // Sphere���g�iIGameObject*�j
		collisionObject_          // Sphere�̃R���W����
	);


	// todo for test : BurretPhysics���g�����V���������蔻��ƃR���W�����ݒ�
	//btCollisionShape* initialShape = new btSphereShape(INITIAL_RADIUS);
	//btVector3 localInertia(0, 0, 0); // �������[�����g : ��]�^���ɂ����镨�̂̉�]�̂��ɂ���
	//initialShape->calculateLocalInertia(INITIAL_MASS, localInertia);


	// �L�����R���̏���
	charaCon_.Init(15.0f, 15.0f, transform_.m_position);
	return true;
}

void Sphere::Update()
{
	// �O�̍��W��ۑ�
	beforePosition_ = transform_.m_position;

	Move();
	Rotation();
	SetGravity();
	sphereRender_.Update();
	collisionObject_->SetPosition(transform_.m_position);
	collisionObject_->SetRotation(transform_.m_rotation);
	collisionObject_->Update();
}

void Sphere::Render(RenderContext& rc) 
{
	sphereRender_.Draw(rc);
}


void Sphere::Move() 
{
	//�ړ������̌v�Z
	moveSpeedMultiply_ = ALWAYS_SPEED * (INITIAL_RADIUS / radius_);

	// �ړ����������Ƃɑ��x���o��
	moveSpeed_.x = moveDirection_.x * moveSpeedMultiply_;
	moveSpeed_.z = moveDirection_.z * moveSpeedMultiply_;

	// �ړ���
	transform_.m_localPosition = charaCon_.Execute(moveSpeed_, 1.0f / 60.0f);
	
	// Transform��Update�@���@�ړ�����X�V
	transform_.UpdateTransform();

	// �G�`������ɍ��W��������B
	sphereRender_.SetPosition(transform_.m_position);
}


void Sphere::Rotation()
{
	// ��]���x���v�Z
	const float rotationSpeed = ALWAYS_SPEED * (moveSpeedMultiply_ / ALWAYS_SPEED);

	// �O�̍��W�ƍ��̍��W�̍����v�Z
	Vector3 move = transform_.m_position - beforePosition_;

	//�ړ����S������Ă��Ȃ��ꍇ�A������Ԃ�
	if (fabsf(move.x) < 0.001f && fabsf(move.z) < 0.001f) { return;	}

	// �ړ��ʂ����߂�
	float length = move.Length();

	//���K��
	move.Normalize();

	// �O�ς����߂�
	vertical_ = Cross(Vector3::AxisY, move);
	// �O�σx�N�g�������Ƃɉ�]�ʂ����߂�
	Quaternion rot;
	rot.SetRotationDeg(vertical_, length * rotationSpeed / 20);

	//���߂��N�H�[�^�j�I������Z����
	transform_.m_localRotation.Multiply(transform_.m_localRotation, rot);
	
	transform_.UpdateTransform();
	sphereRender_.SetRotation(transform_.m_rotation);
}


void Sphere::SetGravity()
{
	// �n�ʂɕt���Ă�����
	if (charaCon_.IsOnGround())
	{
		// �d�͂𖳂����B
		moveSpeed_.y = 0.0f;
	}

	// �n�ʂɕt���Ă��Ȃ�������B
	else
	{
		// �d�͂𔭐�������
		moveSpeed_.y -= 10.0f;
	}

	// �L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	transform_.m_localPosition = charaCon_.Execute(moveSpeed_, 1.0f / 60.0f);
	// �g�����X�t�H�[���̍X�V
	transform_.UpdateTransform();

	// �G�`������ɍ��W��������B
	sphereRender_.SetPosition(transform_.m_position);
}



void Sphere::SetParent(AttachableObject* attachableObject)
{
	attachableObject->GetTransform()->SetParent(&transform_);

	// ��̃R�[�h���킩��₷����������
	//Transform* parentTransform = this->GetTransform(); // Sphere�̃g�����X�t�H�[�����擾
	//Transform* childTransform = attachableObject->GetTransform(); // AttachableObject�̃g�����X�t�H�[�����擾

	//childTransform->SetParent(parentTransform);
}




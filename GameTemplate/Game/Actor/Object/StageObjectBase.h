/**
 * StageObjectBase.h
 * �X�e�[�W�ɔz�u�����I�u�W�F�N�g1����������N���X
 */
#pragma once
#include "Actor/Actor.h"


class StageObjectBase : public Actor
{
protected:
	Vector3 colliderPivot_; // �R���C�_�[�̏����ʒu
	Vector3 colliderSize_; // �R���C�_�[�̑傫��
	std::string assetName_; // �A�Z�b�g�̖��O
	int attachableValue_; // �z���\���ǂ���
	int objectSize_; // �z���\�T�C�Y
	int grouthAmount_; // ��̃T�C�Y�̑�����

	// �ÓI�ȓ����蔻��(�����I�ɓ��������Ƃ������Ƃ��������p)
	PhysicsStaticObject* physicsStaticObject_ = nullptr;
	// �S�[�X�g�ȓ����蔻��(�����I�ɂ�����Ȃ�)
	CollisionObject* collisionObject_ = nullptr;


public:
	/**
	 * @brief �R���X�g���N�^
	 */
	StageObjectBase() {};
	/**
	 * @brief �f�X�g���N�^
	 */
	~StageObjectBase() {};

	virtual bool Start() override;
	virtual void Update() override {};
	virtual void Render(RenderContext& rc) override {};

	
public:
	// @todo for test
	inline PhysicsStaticObject* GetPhysicsStaticObject()
	{
		return physicsStaticObject_;
	}


public:
	/**
	 * @brief ������
	 * @note  NewGO��������ɐ�ΌĂ�ł��������I�I�I�I
	 */
	void Initialize(const int attachValue_, const Vector3& position, const Vector3& scale, const Quaternion& rotation, const float size, const std::string& assetName, const int grouthAmount, const Vector3& colliderPivot, const Vector3& colliderSize)
	{
		transform_.m_localPosition = position; // ���W
		transform_.m_localScale = scale; // �傫��
		transform_.m_rotation = rotation; // ��]

		attachableValue_ = attachValue_; // �z���\���ǂ���
		objectSize_ = size; // �I�u�W�F�N�g�̋z���\�T�C�Y
		assetName_ = assetName; // �A�Z�b�g�̖��O
		grouthAmount_ = grouthAmount; // ��̃T�C�Y�̑�����
		colliderPivot_ = colliderPivot; // �R���C�_�[�̋N�_�̍��W
		colliderSize_ = colliderSize; // �R���C�_�[�̑傫��

	}


public:
	inline int GetObjectSize() 
	{
		return objectSize_;
	}


private:
	/* �I�u�W�F�N�g�̃X�P�[���ƃR���W�����̃T�C�Y�ŏo���X�P�[���̍��ق����߂� */
	Vector3 ApplyCollisionSizeDelta(Vector3 scale);

	/* ���߂����ق��l�����ꂽ�l����Z */
	Vector3 Multply(Vector3 a, Vector3 b);
};
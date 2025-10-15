/**
 * Transform.cpp
 * ���W�A��]�A�g�k���܂Ƃ߂�����
 * �e�q�\�����������W�v�Z�Ȃǂ����₷��
 */
#include "stdafx.h"
#include "Transform.h"


Transform::Transform()
	:m_position(Vector3::Zero)
	, m_localPosition(Vector3::Zero)
	, m_scale(Vector3::One)
	, m_localScale(Vector3::One)
	, m_rotation(Quaternion::Identity)
	, m_localRotation(Quaternion::Identity)
	, m_rotationMatrix_(Matrix::Identity)
	, m_worldMatrix_(Matrix::Identity)
	, m_parent_(nullptr)
{
	m_children_.clear();
}

Transform::~Transform()
{
	if (m_parent_) {
		m_parent_->RemoveChild(this);
	}
	Release();
}

void Transform::UpdateTransform()
{
	
	if (m_parent_) {
		//���W�v�Z
		Matrix localPos;
		localPos.MakeTranslation(m_localPosition);

		Matrix pos;
		pos.Multiply(localPos, m_parent_->m_worldMatrix_);

		//�������s�ړ��̕���������Ă邾��
		m_position.x = pos.m[3][0];
		m_position.y = pos.m[3][1];
		m_position.z = pos.m[3][2];

		//�X�P�[��
		m_scale.x = m_localScale.x * m_parent_->m_scale.x;
		m_scale.y = m_localScale.y * m_parent_->m_scale.y;
		m_scale.z = m_localScale.z * m_parent_->m_scale.z;

		//��]
		m_rotation = m_parent_->m_rotation * m_localRotation;

	}
	else
	{
		//���[�J���̒l�����̂܂܃R�s�[
		m_position = m_localPosition;
		m_scale = m_localScale;
		m_rotation = m_localRotation;
	}

	//��]�s��
	m_rotationMatrix_.MakeRotationFromQuaternion(m_rotation);
	//���[���h�s��X�V
	UpdateWorldMatrix();
}

void Transform::UpdateWorldMatrix()
{
	Matrix scal, pos, world;
	scal.MakeScaling(m_scale);
	pos.MakeTranslation(m_position);

	world.Multiply(scal, m_rotationMatrix_);
	m_worldMatrix_.Multiply(world, pos);

	//�q���X�V
	for (Transform* child : m_children_)
	{
		child->UpdateTransform();
	}
}


void Transform::Release()
{
	//�C�e���[�^����
	std::vector<Transform*>::iterator it = m_children_.begin();
	//vector�̏I���܂ŉ�
	while (it != m_children_.end())
	{
		//�q�g�����X�t�H�[������̕R�Â����O��
		(*it)->m_parent_ = nullptr;
		//�q�g�����X�t�H�[���ւ̕R�Â����O��
		m_children_.erase(it);
		//�C�e���[�^��i�߂�
		++it;
	}
	//�O�̂��߁Hvector�̗v�f��S�폜
	m_children_.clear();
}

void Transform::RemoveChild(Transform* t)
{
	//�C�e���[�^����
	std::vector<Transform*>::iterator it = m_children_.begin();
	//vector����
	while (it != m_children_.end())
	{
		//�C�e���[�^����q�g�����X�t�H�[���̃|�C���^���󂯎��
		Transform* child = (*it);
		//�󂯎�����q�g�����X�t�H�[�������Ă��ꂽ���Ȃ�
		if (child == t)
		{
			//�R�Â����O��
			child->m_parent_ = nullptr;
			m_children_.erase(it);
			//�������I��
			return;
		}

		//�C�e���[�^��i�߂�
		++it;
	}
}
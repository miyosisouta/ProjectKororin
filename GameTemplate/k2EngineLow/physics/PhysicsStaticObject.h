/*!
 * @brief	�ÓI�I�u�W�F�N�g�B
 */

#pragma once

#include "physics/MeshCollider.h"
#include "physics/RigidBody.h"

namespace nsK2EngineLow {
	/// <summary>
	/// �ÓI�����I�u�W�F�N�g
	/// </summary>
	class PhysicsStaticObject : public Noncopyable {
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		PhysicsStaticObject();
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~PhysicsStaticObject();
		/// <summary>
		/// ���f������̐ÓI�I�u�W�F�N�g�̍쐬�B
		/// </summary>
		/// <param name="model">���f��</param>
		/// <param name="worldMatrix">���[���h�s��</param>
		/// <param name="restitution">������</param>
		void CreateFromModel(Model& model, const Matrix& worldMatrix, const float restitution = 0.0f);

		/**
		 * @brief �ÓI�{�b�N�X�R���C�_�[���쐬���܂��B
		 * @param pos �R���C�_�[�̒��S�ʒu��\���x�N�g���B
		 * @param size �R���C�_�[�̃T�C�Y��\���x�N�g���B
		 * @param worldMatrix ���[���h��Ԃł̃R���C�_�[�̕ϊ��s��B
		 * @param restitution �����́i�f�t�H���g��0.0f�j�B
		 */
		void CreateBoxCollider(const Vector3& pos, const Vector3& size, const Matrix& worldMatrix, const float restitution = 0.0f);
		/// <summary>
		/// ���C�͂�ݒ肷��B
		/// </summary>
		/// <param name="friction">���C�́B10���ő�l�B</param>
		void SetFriction(float friction)
		{
			m_rigidBody.SetFriction(friction);
		}
		btCollisionObject* GetbtCollisionObject()
		{
			return m_rigidBody.GetBody();
		}
		/// <summary>
		/// ���̂�j���B
		/// </summary>
		void Release()
		{
			m_rigidBody.Release();
		}

		// @todo for test
		BoxCollider* GetCollider()
		{
			return &m_boxCollider;
		}
		RigidBody* GetRigidBody()
		{
			return &m_rigidBody;
		}

	private:
		MeshCollider m_meshCollider;		//���b�V���R���C�_�[�B
		BoxCollider m_boxCollider;

		RigidBody m_rigidBody;				//���́B
	};
}
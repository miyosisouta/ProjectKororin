/**
 * Transform.h
 * ���W�A��]�A�g�k���܂Ƃ߂�����
 * �e�q�\�����������W�v�Z�Ȃǂ����₷��
 */
#pragma once


/**
 * Transform�N���X
 */
class Transform : public Noncopyable
{
public:
	/** ���g�̌��݂̃p�����[�^ */
	Vector3 m_localPosition;
	Quaternion m_localRotation;
	Vector3 m_localScale;

	/** �e�g�����X�t�H�[�����l�������p�����[�^ */
	// ���_�ƂȂ����
	Vector3 m_position;
	Quaternion m_rotation;
	Vector3 m_scale;


	/**
	 * private�ϐ�
	 * ��{�I�Ɋ֐��ȊO����M��K�v�Ȃ�
	 */
private:
	Matrix m_rotationMatrix_;
	Matrix m_worldMatrix_;

	Transform* m_parent_;
	std::vector<Transform*> m_children_;


public:
	Transform();
	~Transform();

	//�X�V����
	void UpdateTransform();
	//���[���h�s��X�V�AUpdateTransform�̕��ŌĂ΂��̂ŌĂяo���K�v�Ȃ�
	void UpdateWorldMatrix();

	//�S�Ă̎q�g�����X�t�H�[���Ƃ̕R�Â����O��
	void Release();

	//����̎q�g�����X�t�H�[���Ƃ̕R�Â����O��
	void RemoveChild(Transform* t);

	//�e�g�����X�t�H�[����ݒ�
	void SetParent(Transform* p)
	{
		m_parent_ = p;
		m_parent_->m_children_.push_back(this);
	}
};
#include "stdafx.h"
#include "InputSystem.h"
#include "Actor/Sphere.h"


bool InputSystem::Start()
{
	return true;
}


void InputSystem::Update()
{
	// ����^�[�Q�b�g�����Ȃ��ꍇ������Ԃ�
	if (target_ == nullptr) {
		return;
	}

	target_->SetMoveDirection(GetStickL());
}


void InputSystem::Render(RenderContext& rc)
{

}


Vector3 InputSystem::GetStickL()
{
	Vector3 stickL;
	/* �X�e�B�b�N�̓��͗ʂ�ۑ�*/
	stickL.x = GetPad()->GetLStickXF();
	stickL.y = GetPad()->GetLStickYF();

	/*�J�����̑O�����ƉE�����̃x�N�g���������Ă���*/
	/*�v���C���[���ǂ̕����Ɉړ����邩�����߂�*/
	Vector3 forward = g_camera3D->GetForward(); //!<�J�����̑O�����̃x�N�g��
	Vector3 right = g_camera3D->GetRight(); //!<�J�����̉E�����̃x�N�g��

	/*�v���C���[�͒n�ʏ�ł݈̂ړ�����*/
	forward.y = 0.0f;
	right.y = 0.0f;

	/*�ړ����x�̌v�Z*/
	forward *= stickL.y;
	right *= stickL.x;

	// 0�`1�͈̔͂ɕϊ�
	Vector3 direction = right + forward;
	direction.Normalize();

	return direction;
}
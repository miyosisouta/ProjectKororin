#include "stdafx.h"
#include "SphereCamera.h"
#include "Actor/Sphere/sphere.h"
#include "Camera/CameraBase.h"

bool SphereCamera::Start() 
{
	m_sphere_ = FindGO<Sphere>("sphere");

	// �J�����̏����n�_��ݒ�
	transform_.m_localPosition = Vector3(0.0f, 50.0f, 200.0f);
	
	//�J�����̋ߕ��ʁE�����ʂ̐ݒ�
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(1000000.0f);

	// transform���X�V
	transform_.UpdateTransform();
	return true;
}


void SphereCamera::Update()
{
	// �J�������X�V
	// �����_���v�Z����
	Vector3 target = m_sphere_->GetTransform()->m_localPosition;

	// �v���C���̑������炿����Ə�𒍎��_�Ƃ���B
	target.y += 50.0f;

	// ���݂̃|�W�V������ۑ�
	Vector3 toCameraPosOld = transform_.m_localPosition;

	// �p�b�h�̓��͂��g���ăJ��������
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	// Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 2.0f * x);
	qRot.Apply(transform_.m_localPosition);

	// X������̉�]
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, transform_.m_localPosition);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * y);
	qRot.Apply(transform_.m_localPosition);

	// �J�����̉�]�̏�����`�F�b�N����
	// �����_���王�_�܂ł̃x�N�g���𐳋K������B
	// ���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
	// �傫�����P�ɂȂ�Ƃ������Ƃ́A�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ������ƁB
	Vector3 toPosDir = transform_.m_localPosition;
	toPosDir.Normalize();

	if (toPosDir.y < -0.2f) {
		// �J���������������
		transform_.m_localPosition = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		// �J����������������
		transform_.m_localPosition = toCameraPosOld;
	}

	// ���_���v�Z����
	Vector3 pos = target + transform_.m_localPosition;

	// �J�����̒����_�E���_�̐ݒ�
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	// �J�����̍X�V
	g_camera3D->Update();
	transform_.UpdateTransform();

};
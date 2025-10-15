#include "stdafx.h"
#include "SphereCamera.h"
#include "Actor/sphere.h"

bool SphereCamera::Start() 
{
	m_sphere_ = FindGO<Sphere>("sphere");
	m_cameraPos_.Set(0.0f, 50.0f, -200.0f);

	//�J�����̋ߕ��ʁE�����ʂ̐ݒ�
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(1000000.0f);
	return true;
}


void SphereCamera::Update()
{
	// �J�������X�V
	// �����_���v�Z����
	Vector3 target = m_sphere_->GetTransform()->m_position;

	// �v���C���̑������炿����Ə�𒍎��_�Ƃ���B
	target.y += 50.0f;

	Vector3 toCameraPosOld = m_cameraPos_;
	// �p�b�h�̓��͂��g���ăJ��������
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	// Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 2.0f * x);
	qRot.Apply(m_cameraPos_);

	// X������̉�]
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_cameraPos_);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * y);
	qRot.Apply(m_cameraPos_);

	// �J�����̉�]�̏�����`�F�b�N����
	// �����_���王�_�܂ł̃x�N�g���𐳋K������B
	// ���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
	// �傫�����P�ɂȂ�Ƃ������Ƃ́A�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ������ƁB
	Vector3 toPosDir = m_cameraPos_;
	toPosDir.Normalize();

	if (toPosDir.y < -0.2f) {
		// �J���������������
		m_cameraPos_ = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		// �J����������������
		m_cameraPos_ = toCameraPosOld;
	}

	// ���_���v�Z����
	Vector3 pos = target + m_cameraPos_;

	// �J�����̒����_�E���_�̐ݒ�
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	// �J�����̍X�V
	g_camera3D->Update();
};
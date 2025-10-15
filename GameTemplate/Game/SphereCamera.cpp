#include "stdafx.h"
#include "SphereCamera.h"
#include "Actor/sphere.h"

bool SphereCamera::Start() 
{
	m_sphere_ = FindGO<Sphere>("sphere");
	m_cameraPos_.Set(0.0f, 50.0f, -200.0f);

	//カメラの近平面・遠平面の設定
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(1000000.0f);
	return true;
}


void SphereCamera::Update()
{
	// カメラを更新
	// 注視点を計算する
	Vector3 target = m_sphere_->GetTransform()->m_position;

	// プレイヤの足元からちょっと上を注視点とする。
	target.y += 50.0f;

	Vector3 toCameraPosOld = m_cameraPos_;
	// パッドの入力を使ってカメラを回す
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	// Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 2.0f * x);
	qRot.Apply(m_cameraPos_);

	// X軸周りの回転
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_cameraPos_);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * y);
	qRot.Apply(m_cameraPos_);

	// カメラの回転の上限をチェックする
	// 注視点から視点までのベクトルを正規化する。
	// 正規化すると、ベクトルの大きさが１になる。
	// 大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
	Vector3 toPosDir = m_cameraPos_;
	toPosDir.Normalize();

	if (toPosDir.y < -0.2f) {
		// カメラが上向きすぎ
		m_cameraPos_ = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		// カメラが下向きすぎ
		m_cameraPos_ = toCameraPosOld;
	}

	// 視点を計算する
	Vector3 pos = target + m_cameraPos_;

	// カメラの注視点・視点の設定
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	// カメラの更新
	g_camera3D->Update();
};
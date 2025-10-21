#include "stdafx.h"
#include "SphereCamera.h"
#include "Actor/Sphere/sphere.h"
#include "Camera/CameraBase.h"

bool SphereCamera::Start() 
{
	m_sphere_ = FindGO<Sphere>("sphere");

	// カメラの初期地点を設定
	transform_.m_localPosition = Vector3(0.0f, 50.0f, 200.0f);
	
	//カメラの近平面・遠平面の設定
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(1000000.0f);

	// transformを更新
	transform_.UpdateTransform();
	return true;
}


void SphereCamera::Update()
{
	// カメラを更新
	// 注視点を計算する
	Vector3 target = m_sphere_->GetTransform()->m_localPosition;

	// プレイヤの足元からちょっと上を注視点とする。
	target.y += 50.0f;

	// 現在のポジションを保存
	Vector3 toCameraPosOld = transform_.m_localPosition;

	// パッドの入力を使ってカメラを回す
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	// Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 2.0f * x);
	qRot.Apply(transform_.m_localPosition);

	// X軸周りの回転
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, transform_.m_localPosition);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * y);
	qRot.Apply(transform_.m_localPosition);

	// カメラの回転の上限をチェックする
	// 注視点から視点までのベクトルを正規化する。
	// 正規化すると、ベクトルの大きさが１になる。
	// 大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
	Vector3 toPosDir = transform_.m_localPosition;
	toPosDir.Normalize();

	if (toPosDir.y < -0.2f) {
		// カメラが上向きすぎ
		transform_.m_localPosition = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		// カメラが下向きすぎ
		transform_.m_localPosition = toCameraPosOld;
	}

	// 視点を計算する
	Vector3 pos = target + transform_.m_localPosition;

	// カメラの注視点・視点の設定
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	// カメラの更新
	g_camera3D->Update();
	transform_.UpdateTransform();

};
#include "stdafx.h"
#include "SphereCamera.h"
#include "Actor/Sphere/sphere.h"

namespace 
{
	Vector3 INIT_POSITION = Vector3::Zero;
	Vector3 CAMERA_LEVEL_UP_OFFSET = Vector3(0.0f, 25.0f, 100.0f); // 塊との距離
	float TARGET_DISTANCE = 200.0f;
	float TARGET_HEIGHT = 50.0f; // 塊との高さの差
	float ZOOM_OUT_TIME = 2.5f; // ズームアウトにかける時間
}

bool SphereCamera::Start() 
{
	// カメラの初期設定
	transform_.m_localPosition = Vector3(0.0f, TARGET_HEIGHT, TARGET_DISTANCE);
	//近平面・遠平面の設定
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(1000000.0f);

	// Lerp先を設定
	nextPosition_ = transform_.m_localPosition;

	// トランスフォームを更新
	transform_.UpdateTransform();
	return true;
}


void SphereCamera::Update()
{
	/* 塊の大きさのレベルによってカメラを塊から遠ざける */
	auto* currentSphereLevel = sphere_->GetStatus(); // 塊の現在のレベルを取得
	if (CheckForLevelUp(currentSphereLevel->GetLevel())) // カメラを遠ざける条件が満たされている場合
	{
		InitZoomOut(); // 移動先の設定
		calclerpValue_.InitCalcTime(ZOOM_OUT_TIME); // 移動時間の設定
	}


	Vector3 target = sphere_->GetTransform()->m_localPosition;	// 注視点を計算
	target.y += TARGET_HEIGHT; 	// 塊の中心の少し上を注視点とする。
	Vector3 toCameraPosOld = transform_.m_localPosition; // 現在のポジションを保存


	if (IsActiveCamera())
	{
		// パッドの入力を使ってカメラを回す
		float x = g_pad[0]->GetRStickXF();
		float y = g_pad[0]->GetRStickYF();


		// Y軸の回転
		Quaternion qRot;
		qRot.SetRotationDeg(Vector3::AxisY, 2.0f * x);
		qRot.Apply(transform_.m_localPosition);
		// X軸の回転
		Vector3 axisX;
		axisX.Cross(Vector3::AxisY, transform_.m_localPosition);
		axisX.Normalize();
		qRot.SetRotationDeg(axisX, 2.0f * y);
		qRot.Apply(transform_.m_localPosition);

	}

	// 注視点から視点までのベクトルを正規化
	Vector3 toPosDir = transform_.m_localPosition;
	toPosDir.Normalize();

	// カメラの回転の上限をチェック
	if (toPosDir.y < -0.2f) { transform_.m_localPosition = toCameraPosOld; } // カメラが上向きすぎ
	else if (toPosDir.y > 0.9f) { transform_.m_localPosition = toCameraPosOld; } // カメラが下向きすぎ

	CalcZoomOut();

	// 視点を計算
	Vector3 pos = target + transform_.m_localPosition;

	// カメラの注視点・視点の設定
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	// カメラの更新
	g_camera3D->Update();
	transform_.UpdateTransform();
}

void SphereCamera::InitZoomOut()
{
	nextPosition_ = CAMERA_LEVEL_UP_OFFSET * nextNeedLevel_; // 移動先の設定
	nextNeedLevel_++; // 移動先を設定したら
	lerpCurrentMovement_ = Vector3::Zero; // Lerp時の移動量を初期化
	beforeFrameMovement_ = Vector3::Zero; // Lerp時の1フレームの移動量を初期化
}

void SphereCamera::CalcZoomOut()
{
	Vector3 deltaPosition = nextPosition_ - lerpCurrentMovement_; // Lerpにて動かした移動量と動かしたい座標の差
	deltaPosition.Normalize(); // 正規化

	if (deltaPosition.y < 0.01f || deltaPosition.z < 0.01f)
	{
		const float calcValue = calclerpValue_.CalcUpdate();
		// nextPositionの設定がされたとき、処理がされる
		lerpCurrentMovement_.Lerp(calcValue, INIT_POSITION, nextPosition_); // 計算を始めてからの移動量を計算
		lerpCurrentMovement_ -= beforeFrameMovement_; // 前フレームの移動量を減算
		beforeFrameMovement_ = lerpCurrentMovement_; // 今のフレームの移動量を保管
	}

	// 1フレームごとのズームアウト量をローカル座標に加算
	transform_.m_localPosition += lerpCurrentMovement_;
}

bool ResultCamera::Start()
{
	Vector3 target = Vector3::Zero;

	Vector3 pos = target + Vector3(0.0f, 0.0f, TARGET_DISTANCE);

	//近平面・遠平面の設定
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(1000000.0f);

	// カメラの注視点・視点の設定
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	// トランスフォームを更新
	transform_.UpdateTransform();

	return true;
}

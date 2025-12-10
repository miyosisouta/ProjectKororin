#include "stdafx.h"
#include "SphereInputSystem.h"
#include "Actor/Sphere/Sphere.h"


bool SphereInputSystem::Start()
{
	return true;
}


void SphereInputSystem::Update()
{
	// 操作ターゲットがいない場合処理を返す
	if (target_ == nullptr) {
		return;
	}

	target_->SetMoveDirection(GetStickL());
}


void SphereInputSystem::Render(RenderContext& rc)
{

}


Vector3 SphereInputSystem::GetStickL()
{
	Vector3 stickL;
	/* スティックの入力量を保存*/
	stickL.x = GetPad()->GetLStickXF();
	stickL.y = GetPad()->GetLStickYF();

	/*カメラの前方向と右方向のベクトルを持ってくる*/
	/*プレイヤーがどの方向に移動するかを決める*/
	Vector3 forward = g_camera3D->GetForward(); //!<カメラの前方向のベクトル
	Vector3 right = g_camera3D->GetRight(); //!<カメラの右方向のベクトル

	/*プレイヤーは地面上でのみ移動する*/
	forward.y = 0.0f;
	right.y = 0.0f;

	/*移動速度の計算*/
	forward *= stickL.y;
	right *= stickL.x;

	// 0～1の範囲に変換
	Vector3 direction = right + forward;
	direction.Normalize();

	return direction;
}




bool TitleInputSyste::Start()
{
	return true;
}


void TitleInputSyste::Update()
{
	// 操作ターゲットがいない場合処理を返す
	if (target_ == nullptr) {
		return;
	}

	if (moveDirection_.LengthSq() > 0.01f) {
		target_->SetMoveDirection(moveDirection_);
	} else {
		target_->SetMoveDirection(Vector3::Zero);
	}
}
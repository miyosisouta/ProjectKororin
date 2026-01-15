#include "stdafx.h"
#include "Player.h"
#include "Sphere.h"

bool Player::Start()
{
	// モデルの初期化
	modelRender_.Init("Assets/modelData/player/unityChan.tkm");

	////アニメーションを設定
	//m_playerAnimClips[enPlayerAnimClips_Idle].Load("Assets/animData/player/idle.tka");
	//m_playerAnimClips[enPlayerAnimClips_Idle].SetLoopFlag(true);
	//m_playerAnimClips[enPlayerAnimClips_Run].Load("Assets/animData/player/Run.tka");
	//m_playerAnimClips[enPlayerAnimClips_Run].SetLoopFlag(true);

	// キャラコンの座標とサイズを設定
	m_charaCon.Init(25.0f, 75.0f, transform_.m_position);
	return true;
}

void Player::Update()
{
	// トランスフォームの更新
	transform_.UpdateTransform();

	// キャラコンの設定
	m_charaCon.SetPosition(transform_.m_position);
	transform_.m_localPosition = m_charaCon.Execute(
		moveSpeed_,
		g_gameTime->GetFrameDeltaTime()
	);

	// トランスフォームを更新
	transform_.UpdateTransform();

	// モデルにTransformを設定・更新
	modelRender_.SetRotation(transform_.m_rotation);
	modelRender_.SetPosition(transform_.m_position);
	modelRender_.Update();

	// 移動処理
	//Move();
}

void Player::Render(RenderContext& rc)
{
	modelRender_.Draw(rc);
}

void Player::Move()
{

}

void Player::ManageState()
{
	//ステートごとの処理
	switch (m_playerState)
	{
	case PlayerState::Idle:
		break;
	case PlayerState::Run:
		Move();
		break;
	}
}
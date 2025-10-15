#include "stdafx.h"
#include "Player.h"
#include "Sphere.h"

bool Player::Start() 
{
	modelRender_.Init("Assets/modelData/player/unityChan.tkm");

	//m_playerAnimClips[enPlayerAnimClips_Idle].Load("Assets/animData/player/idle.tka");
	//m_playerAnimClips[enPlayerAnimClips_Idle].SetLoopFlag(true);
	//m_playerAnimClips[enPlayerAnimClips_Run].Load("Assets/animData/player/Run.tka");
	//m_playerAnimClips[enPlayerAnimClips_Run].SetLoopFlag(true);

	m_charaCon.Init(25.0f, 75.0f, transform_.m_position);
	return true;
}

void Player::Update() 
{
	/*キャラコンを使用して*/
	/*プレイヤーの座標とモデルの座標を更新する*/
	transform_.UpdateTransform();
	m_charaCon.SetPosition(transform_.m_position);
	transform_.m_localPosition = m_charaCon.Execute(
		m_moveSpeed,
		g_gameTime->GetFrameDeltaTime()
	);
	transform_.UpdateTransform();

	modelRender_.SetRotation(transform_.m_rotation);
	modelRender_.SetPosition(transform_.m_position);
	modelRender_.Update();

	Move();
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
	switch (m_playerState) 
	{
	case PlayerState::Idle:
		break;
	case PlayerState::Run:
		Move();
		break;
	}
}
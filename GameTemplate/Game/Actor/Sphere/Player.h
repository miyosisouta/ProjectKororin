/**
 * @file Player.h
 * @brief プレイヤーキャラクターを表すクラスの宣言。
 */
#pragma once
#include "Actor/Actor.h"


class Sphere;
class Player : public Actor
{
private:
	/* プレイヤーのステート */
	struct PlayerState
	{
		enum Enum
		{
			Idle,
			Run,
		};
	};

	/* プレイヤーのアニメーションステート */
	struct PlayerAnimClips
	{
		enum Enum
		{
			None,
			Idle,
			Run,
			Max,
		};
	};


private:
	CharacterController m_charaCon;	//!< キャラクターコントローラ
	AnimationClip m_playerAnimClips[PlayerAnimClips::Max]; //!< アニメーションクリップ
	PlayerState::Enum m_playerState = PlayerState::Idle; //!< プレイヤーステート
	Sphere* Sphere_ = nullptr; //!< 塊

	Vector3 moveSpeed_ = Vector3::Zero; //!< 移動速度


public:
	/* コンストラクタ */
	Player() {};
	/* デストラクタ */
	~Player() {};

	/* スタート処理 */
	bool Start() override;
	/* 更新処理 */
	void Update() override;
	/* 描画処理 */
	void Render(RenderContext& rc) override;

private:
	/* 移動処理 */
	void Move();

	/* ステート管理 */
	void ManageState();

	/* アニメーション設定 */
	void SetAnimation(PlayerAnimClips::Enum animationClip, std::string animationFileName, bool loopFlag);
};


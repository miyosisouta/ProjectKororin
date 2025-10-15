/**
 * @file Player.h
 * @brief プレイヤーキャラクターを表すクラスの宣言。
 */
#pragma once
#include "Actor.h"


class Sphere;
class Player : public Actor
{
private:
	/**
	 * @brief プレイヤーの状態を表す列挙体。
	 */
	struct PlayerState
	{
		enum Enum
		{
			Idle,
			Run,
		};
	};

	/**
	 * @brief プレイヤーのアニメーションクリップ種別。
	 */
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
	CharacterController m_charaCon; //!< キャラクターコントローラ
	AnimationClip m_playerAnimClips[PlayerAnimClips::Max];
	PlayerState::Enum m_playerState = PlayerState::Idle;
	Sphere* m_sphere = nullptr; //!< ボール

	Vector3 m_moveSpeed = Vector3::Zero; //!< 移動速度


public:
	/**
	 * @brief コンストラクタ。
	 */
	Player() {};
	/**
	 * @brief デストラクタ。
	 */
	~Player() {};


	/**
	 * @brief 初期化処理。
	 * @return 成功した場合は true。
	 */
	bool Start();
	/**
	 * @brief 毎フレームの更新処理。
	 */
	void Update();
	/**
	 * @brief 描画処理。
	 * @param rc 描画に使用するレンダーコンテキスト。
	 */
	void Render(RenderContext& rc);


	/**
	 * @brief 移動処理。
	 */
	void Move();

	/**
	 * @brief ステート管理処理。
	 */
	void ManageState();

	/**
	 * @brief アニメーションのセット。
	 * @param animationClip 設定するアニメーションクリップ。
	 * @param animationFileName アニメーションファイル名。
	 * @param loopFlag ループ再生するかどうか。
	 */
	void SetAnimation(PlayerAnimClips::Enum animationClip, std::string animationFileName, bool loopFlag);
};


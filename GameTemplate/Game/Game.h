/**
 * Game.h
 * プレイ中のゲームの流れを処理するクラス
 */

#pragma once
#include "Level3DRender/LevelRender.h"

class GameManager;
class Player;
class Sphere;
class SphereCamera;
class Stage;

class Game : public IGameObject
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	Game() {};
	/**
	* @brief デストラクタ。
	 */
	~Game();


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

public:
	GameManager* m_gameManager = nullptr; //!< ゲーム管理クラスへの参照
	Player* m_player = nullptr; //!< プレイヤーキャラクタ
	Sphere* m_sphere = nullptr; //!< ボール（Sphere）
	SphereCamera* m_sphereCamera = nullptr; //!< ボール追従カメラ
	Stage* m_stage = nullptr; //!< ステージ


public:


};
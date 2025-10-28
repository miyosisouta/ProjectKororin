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
class Canvas;

class Game : public IGameObject
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	Game() {}
	/**
	* @brief デストラクタ。
	 */
	~Game();


	/**
	 * @brief 初期化処理。
	 * @return 成功した場合は true。
	 */
	bool Start()override;
	/**
	 * @brief 毎フレームの更新処理。
	 */
	void Update()override;
	/**
	 * @brief 描画処理
	 */
	void Render(RenderContext& rc)override;

public:
	Player* m_player = nullptr; //!< プレイヤーキャラクタ
	Sphere* sphere_ = nullptr; //!< ボール（Sphere）
	SphereCamera* sphereCamera_ = nullptr; //!< ボール追従カメラ
	Stage* stage_ = nullptr; //!< ステージ
	Canvas* canvas_ = nullptr; //!< キャンバス


private:
	/**
	 * @brief ゲームクリア、ゲームオーバーの判定をここで行う
	 */
	void CheckAndHandleGameState();

};
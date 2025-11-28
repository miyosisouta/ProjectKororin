/**
 * Game.h
 * プレイ中のゲームの流れを処理するクラス
 */
#pragma once
#include "IScene.h"

class Player;
class Sphere;
class SphereCamera;
class Stage;
class Canvas;
class InputSystem;


class GameScene : public IScene
{
	appScene(GameScene);


private:
	enum ResultStep
	{
		Step1,
		Step2,
		Step3,
		Step4,
		Step5,
	};

	struct ResultInformation
	{
		float time;				// クリア時間
		int count;				// くっついた個数
		float scale;			// 大きさ
		bool isSetting = false;	// 情報が設定されたか
	};


private:
	struct InGameState
	{
		enum Enum
		{
			InGame,			// インゲーム中
			InGameFinish,	// インゲーム終了
			Result,			// リザルト表示
		};
	};


public:
	/**
	 * @brief コンストラクタ。
	 */
	GameScene();
	/**
	* @brief デストラクタ。
	 */
	~GameScene();


	/** 初期化処理 */
	virtual bool Start()override;
	/** 更新処理 */
	virtual void Update()override;
	/** 描画処理 */
	virtual void Render(RenderContext& rc)override;

	virtual bool RequestID(uint32_t& id)override;

private:
	/**
	 * 残り時間によってα値を計算、Fadeクラスにて設定
	 */
	void CalculateFadeAlphaByTime();

private:
	Player* m_player = nullptr; //!< プレイヤーキャラクタ
	Sphere* sphere_ = nullptr; //!< ボール（Sphere）
	SphereCamera* sphereCamera_ = nullptr; //!< ボール追従カメラ
	Stage* stage_ = nullptr; //!< ステージ
	Canvas* canvas_ = nullptr; //!< キャンバス
	InputSystem* inputSystem_ = nullptr;

	InGameState::Enum gameState_ = InGameState::InGame;	// 現在処理の段階
	ResultInformation resultInformation_;				// リザルト時に必要なデータ
	ResultStep step_ = ResultStep::Step1;				// リザルトの処理段階

	FontRender resultGuidanceSizeText_;			// 「大きさ」の表示
	FontRender resultGuidanceGoalTime_;			// 「経過時間」の表示
	FontRender resultGuidanceAttachCountText_;	// 「モノ」の表示
	FontRender resultSphereSizeText_;			// 塊の大きさのテキスト
	FontRender goalTimeText_;					// 塊の目標サイズ達成時の時間のテキスト
	FontRender attachableObjectCountText_;		// 吸着したオブジェクトの個数のテキスト
	FontRender failureText_;					// 失敗時のテキスト
	FontRender buttonText_;						// ボタンをおしてね！のテキスト

	SpriteRender buttonSprite_;					// Aボタンのイラスト

	ModelRender blackOutObject_;				// 失敗時の背景用オブジェクト
	
	bool isResultTextRender_ = false;	// リザルトテキストを表示するか	
	int goalMinuteTime_ = 0.0f;		// クリアタイム : 分
	int goalSecondTime_ = 0.0f;		// クリアタイム : 秒
	float goalElapsedTime_ = 0.0f;	// クリアタイム
	float elapsedTime_ = 0.0f;		// 経過時間
	float whiteOutAlpha = 0.0f;

private:
	bool isNextScene_ = false;		// 次のシーンに遷移するかどうか
};
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



/**
 * @brief ゲームシーン内部でのみ使用します
 */
class GameScene;
namespace _internal
{
	struct ResultInformation
	{
		float time;				// クリア時間
		int count;				// くっついた個数
		float scale;			// 大きさ
		bool isSetting = false;	// 情報が設定されたか
	};


	class Result
	{
	private:
		using EnterFunc = void (*)(Result*);
		using UpdateFunc = void (*)(Result*);
		using ExitFunc = void (*)(Result*);


	private:
		struct State
		{
			EnterFunc enter = nullptr;
			UpdateFunc update = nullptr;
			ExitFunc exit = nullptr;
		};


	private:
		struct Step
		{
			enum Enum
			{
				Step1,
				Step2,
				Step3,
				Step4,
				Step5,
				Max,
				Invalid = Max,
			};
		};


	private:
		Step::Enum currentStep_ = Step::Step1;
		Step::Enum nextStep_ = Step::Invalid;
		ResultInformation information_;
		GameScene* owner_;

		std::unique_ptr<FontRender> resultGuidanceSizeText_ = nullptr;			// 「大きさ」の表示
		std::unique_ptr<FontRender> resultGuidanceGoalTime_ = nullptr;			// 「経過時間」の表示
		std::unique_ptr<FontRender> resultGuidanceAttachCountText_ = nullptr;	// 「モノ」の表示
		std::unique_ptr<FontRender> resultSphereSizeText_ = nullptr;			// 塊の大きさのテキスト
		std::unique_ptr<FontRender> goalTimeText_ = nullptr;					// 塊の目標サイズ達成時の時間のテキスト
		std::unique_ptr<FontRender> attachableObjectCountText_ = nullptr;		// 吸着したオブジェクトの個数のテキスト
		std::unique_ptr<FontRender> failureTexts_[5];							// 失敗時のテキスト
		std::unique_ptr<FontRender> buttonText_ = nullptr;						// ボタンをおしてね！のテキスト

		std::unique_ptr<SpriteRender> buttonSprite_ = nullptr;					// Aボタンの画像
		std::unique_ptr<SpriteRender> instructionButtonSprite_ = nullptr;		// 指示ボタンの画像
		std::unique_ptr<SpriteRender> textWindowSprite_ = nullptr;				// テキストウィンドウの画像

		std::unique_ptr<ModelRender> blackOutObject_ = nullptr;				// 失敗時の背景用オブジェクト

		CalcLerpValue calclerpValue_;

		bool isMoveBlackOutObject_ = false; // 黒いオブジェクトが移動しているかどうか
		bool isResultTextRender_ = false;	// リザルトテキストを表示するか	
		float elapsedTime_ = 0.0f;		// 経過時間
		uint8_t currentSentenceCount = 0;		// 文の数
		int goalMinuteTime_ = 0.0f;				// クリアタイム : 分
		int goalSecondTime_ = 0.0f;				// クリアタイム : 秒
		
		


	public:
		Result(GameScene* owner);
		~Result();

		void Start();
		void Update();
		void Render(RenderContext& rc);


		void SetInformation(const ResultInformation info) { information_ = info; }

	private:
		static std::array<State, Step::Max> stepList_;


	public:
		static void Initialize();


	private:
		/** Step1 */
		static void EnterStep1(Result* result);
		static void UpdateStep1(Result* result);
		static void ExitStep1(Result* result);

		/** Step2 */
		static void EnterStep2(Result* result);
		static void UpdateStep2(Result* result);
		static void ExitStep2(Result* result);

		/** Step3 */
		static void EnterStep3(Result* result);
		static void UpdateStep3(Result* result);
		static void ExitStep3(Result* result);

		/** Step4 */
		static void EnterStep4(Result* result);
		static void UpdateStep4(Result* result);
		static void ExitStep4(Result* result);

		/** Step5 */
		static void EnterStep5(Result* result);
		static void UpdateStep5(Result* result);
		static void ExitStep5(Result* result);
	};
}






class GameScene : public IScene
{
	appScene(GameScene);

	friend class _internal::Result;

private:
	enum ResultStep
	{
		Step1,
		Step2,
		Step3,
		Step4,
		Step5,
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

	virtual bool RequestID(uint32_t& id, float& waitTime)override;

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

	_internal::Result* result_ = nullptr;		// リザルト表示用

	InGameState::Enum gameState_ = InGameState::InGame;	// 現在処理の段階

	float goalElapsedTime_ = 0.0f;	// クリアタイム
	float whiteOutAlpha = 0.0f;		// フェード用画像のα値

private:
	bool isNextScene_ = false;		// 次のシーンに遷移するかどうか
};
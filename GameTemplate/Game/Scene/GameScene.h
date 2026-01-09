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
class SphereInputSystem;
class InputDetection;
class UICanvas;
class UIIcon;


/**
 * @brief ゲームシーン内部でのみ使用します
 */
class GameScene;
namespace _internal
{
	enum ResultType
	{
		Clear,
		Failure,
		Max,
	};


	class Result
	{
	public:
		/** Step1 */
		static void CommonEnterStep1(Result* result);
		static void CommonUpdateStep1(Result* result);
		static void CommonExitStep1(Result* result);

		/** Step2 */
		static void CommonEnterStep2(Result* result);
		static void CommonUpdateStep2(Result* result);
		static void CommonExitStep2(Result* result);

		/** Step3 */
		static void CommonEnterStep3(Result* result);
		static void CommonUpdateStep3(Result* result);
		static void CommonExitStep3(Result* result);

		/** Step4 */
		static void CommonEnterStep4(Result* result);
		static void CommonUpdateStep4(Result* result);
		static void CommonExitStep4(Result* result);

		/** Step5 */
		static void CommonEnterStep5(Result* result);
		static void CommonUpdateStep5(Result* result);
		static void CommonExitStep5(Result* result);

		/** Step6 */
		static void CommonEnterStep6(Result* result);
		static void CommonUpdateStep6(Result* result);
		static void CommonExitStep6(Result* result);



	protected:
		/* 線形補間の初期化 */
		void SetUpToLerp(Vector3 start, Vector3 end)
		{
			sphereResultInitPos = start;
			sphereResultGoalPos = end;
		}


	protected:
		CalcLerpValue calclerpValue_;						//!< リープ用クラスの変数
		GameScene* owner_;									//!< ゲームシーン
		UICanvas* instructionButtonSprite_ = nullptr;		//!< 指示ボタンの画像
		UICanvas* breakScreenSprite_ = nullptr;				//!< 画面が割れた画像
		UICanvas* titleTransitionSprite_ = nullptr;			//!< タイトルへの画像
		UIIcon* instructionIcon_ = nullptr;					//!< 指示ボタン
		UIIcon* titleTransitionWindowIcon_ = nullptr;		//!< タイトルへウィンドウ
		UIIcon* buttonAIcon_ = nullptr;						//!< Aボタン

		std::unique_ptr<SpriteRender> buttonSprite_ = nullptr;					//!< Aボタンの画像
		std::unique_ptr<SpriteRender> textWindowSprite_ = nullptr;				//!< テキストウィンドウの画像

		std::unique_ptr<ModelRender> blackOutObject_ = nullptr;					//!< 失敗時の背景用オブジェクト


		Vector3 sphereResultGoalPos = Vector3::Zero; //!< リザルト時の塊が移動する座標を格納
		Vector3 sphereResultInitPos = Vector3::Zero; //!< リザルト時の塊の移動前の座標を格納
		bool isMoveBlackOutObject_ = false; //!< 黒いオブジェクトが移動しているかどうか
		bool isResultTextRender_ = false;	//!< リザルトテキストを表示するか
		bool isSphereMoveUp_ = true;		//!< リザルト時塊を上に動かすかどうかのフラグ
		bool isGoNextText_ = true;			//!< 次のテキストを映してよいか
		uint8_t currentSentenceIndex_ = 0;	//!< 文の数
		int goalMinuteTime_ = 0.0f;			//!< クリアタイム : 分
		int goalSecondTime_ = 0.0f;			//!< クリアタイム : 秒
		float elapsedTime_ = 0.0f;		//!< 経過時間




	public:
		Result(GameScene* owner);
		virtual ~Result();

		virtual void Start();
		virtual void Update();
		virtual void Render(RenderContext& rc);

	};


	/*********** クリア用リザルト ***********/

	class ClearResult : public Result
	{
	public:
		struct ClearStep
		{
			enum Enum
			{
				Step1,
				Step2,
				Step3,
				Step4,
				Step5,
				Step6,
				Max,
				Invalid = Max,
			};
		};

		struct ResultInformation
		{
			float time;				//!< クリア時間
			int count;				//!< くっついた個数
			float scale;			//!< 大きさ
			bool isSetting = false;	//!< 情報が設定されたか
		};


	public:
		ClearResult(GameScene* owner);
		~ClearResult()override;

		void Start() override;
		void Update() override;
		void Render(RenderContext& rc) override;


	public:
		void SetInformation(const ResultInformation info) { information_ = info; }


	private:
		/** Step1 */
		static void EnterStep1(ClearResult* result);
		static void UpdateStep1(ClearResult* result);
		static void ExitStep1(ClearResult* result);

		/** Step2 */
		static void EnterStep2(ClearResult* result);
		static void UpdateStep2(ClearResult* result);
		static void ExitStep2(ClearResult* result);

		/** Step3 */
		static void EnterStep3(ClearResult* result);
		static void UpdateStep3(ClearResult* result);
		static void ExitStep3(ClearResult* result);

		/** Step4 */
		static void EnterStep4(ClearResult* result);
		static void UpdateStep4(ClearResult* result);
		static void ExitStep4(ClearResult* result);

		/** Step5 */
		static void EnterStep5(ClearResult* result);
		static void UpdateStep5(ClearResult* result);
		static void ExitStep5(ClearResult* result);

		/** Step6 */
		static void EnterStep6(ClearResult* result);
		static void UpdateStep6(ClearResult* result);
		static void ExitStep6(ClearResult* result);

	private:
		using EnterFunc = void(*)(ClearResult*);
		using UpdateFunc = void(*)(ClearResult*);
		using ExitFunc = void(*)(ClearResult*);

		struct State
		{
			EnterFunc enter = nullptr;
			UpdateFunc update = nullptr;
			ExitFunc exit = nullptr;
		};


	private:
		ResultInformation information_;					//!< リザルト時に必要な情報を持つ構造体
		std::array<State, ClearStep::Max> stepList_;	//!< クリア時の処理の流れ

		ClearStep::Enum currentStep_ = ClearStep::Step1;			//!< 現在のステップ
		ClearStep::Enum nextStep_ = ClearStep::Invalid;				//!< 次のステップ

		UICanvas* scoreWindow_ = nullptr;			//!< スコア表示時の背景画像
		UICanvas* goodRatingStamp_ = nullptr;		//!< 最高評価スタンプの画像
		UICanvas* normalRatingStamp_ = nullptr;		//!< 通常評価スタンプの画像
		UIIcon* scoreWindowIcon_ = nullptr;			//!< スコア表示時の背景画像のアイコン
		UIIcon* goodRatingStampIcon_ = nullptr;		//!< 最高評価スタンプの画像のアイコン
		UIIcon* normalRatingStampIcon_ = nullptr;	//!< 通常評価スタンプの画像のアイコン


		std::unique_ptr<FontRender> resultGuidanceSizeText_ = nullptr;			//!< 「大きさ」の表示
		std::unique_ptr<FontRender> resultGuidanceGoalTime_ = nullptr;			//!< 「経過時間」の表示
		std::unique_ptr<FontRender> resultGuidanceAttachCountText_ = nullptr;	//!< 「モノ」の表示
		std::unique_ptr<FontRender> resultSphereSizeText_ = nullptr;			//!< 塊の大きさのテキスト
		std::unique_ptr<FontRender> goalTimeText_ = nullptr;					//!< 塊の目標サイズ達成時の時間のテキスト
		std::unique_ptr<FontRender> attachableObjectCountText_ = nullptr;		//!< 吸着したオブジェクトの個数のテキスト
		std::unique_ptr<FontRender> clearTexts_[5];								//!< 成功時のテキスト

		uint8_t scoreDrawStep_ = 1;
		uint8_t scaleRate_ = 0;
	};


	/*********** 失敗用リザルト ***********/
	class FailureResult : public Result
	{
		struct FailureStep
		{
			enum Enum
			{
				Step1,
				Step2,
				Step3,
				Step4,
				Step5,
				Step6,
				Max,
				Invalid = Max,
			};
		};

	private:
		/** Step1 */
		static void EnterStep1(FailureResult* result);
		static void UpdateStep1(FailureResult* result);
		static void ExitStep1(FailureResult* result);

		/** Step2 */
		static void EnterStep2(FailureResult* result);
		static void UpdateStep2(FailureResult* result);
		static void ExitStep2(FailureResult* result);

		/** Step3 */
		static void EnterStep3(FailureResult* result);
		static void UpdateStep3(FailureResult* result);
		static void ExitStep3(FailureResult* result);

		/** Step4 */
		static void EnterStep4(FailureResult* result);
		static void UpdateStep4(FailureResult* result);
		static void ExitStep4(FailureResult* result);

		/** Step5 */
		static void EnterStep5(FailureResult* result);
		static void UpdateStep5(FailureResult* result);
		static void ExitStep5(FailureResult* result);

		/** Step6 */
		static void EnterStep6(FailureResult* result);
		static void UpdateStep6(FailureResult* result);
		static void ExitStep6(FailureResult* result);


	private:
		using EnterFunc = void(*)(FailureResult*);
		using UpdateFunc = void(*)(FailureResult*);
		using ExitFunc = void(*)(FailureResult*);

		struct State
		{
			EnterFunc enter = nullptr;
			UpdateFunc update = nullptr;
			ExitFunc exit = nullptr;
		};

	public:
		FailureResult(GameScene* owner);
		~FailureResult() override;

		void Start() override;
		void Update() override;
		void Render(RenderContext& rc) override;


	private:
		std::array<State, FailureStep::Max> stepList_;	//!< クリア時の処理の流れ

		FailureStep::Enum currentStep_ = FailureStep::Step1;			//!< 現在のステップ
		FailureStep::Enum nextStep_ = FailureStep::Invalid;				//!< 次のステップ

		std::unique_ptr<FontRender> failureTexts_[5];							//!< 失敗時のテキスト
		std::unique_ptr<FontRender> buttonText_ = nullptr;						//!< ボタンをおしてね！のテキスト

	};
}




class GameScene : public IScene
{
	appScene(GameScene);

	friend class _internal::Result;
	friend class _internal::ClearResult;
	friend class _internal::FailureResult;

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
	SkyCube* skyCube_ = nullptr; //!< スカイキューブ
	Player* m_player = nullptr; //!< プレイヤーキャラクタ
	Sphere* sphere_ = nullptr; //!< ボール（Sphere）
	SphereCamera* sphereCamera_ = nullptr; //!< ボール追従カメラ
	Stage* stage_ = nullptr; //!< ステージ
	Canvas* canvas_ = nullptr; //!< キャンバス
	SphereInputSystem* sphereInputSystem_ = nullptr; // 塊用インプットシステム
	InputDetection* inputDetection_ = nullptr; // 入力判定用インプットシステム

	_internal::Result* result_ = nullptr;		// リザルト表示用

	InGameState::Enum gameState_ = InGameState::InGame;	// 現在処理の段階

	float goalElapsedTime_ = 0.0f;	// クリアタイム
	float whiteOutAlpha = 0.0f;		// フェード用画像のα値

private:
	bool isNextScene_ = false;		// 次のシーンに遷移するかどうか
};
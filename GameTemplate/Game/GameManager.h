/**
 * @file GameManager.h
 * @brief シーン管理を行う GameManager の宣言。
 * @details タイトル/ゲーム/リザルトなどのシーン状態を保持し、
 *          遷移先の指定や現在シーンの取得を提供します。
 */
#pragma once


class Title;
class Game;
class Result;

class GameManager : public IGameObject
{
public:
	enum SceneState 
	{
		EnNone,   	//!< 未設定/無効状態 (0)
		EnTitle,  	//!< タイトルシーン
		EnGame,   	//!< ゲームシーン
		EnResult, 	//!< リザルトシーン
		EnFade,   	//!< フェード中の擬似シーン
	};

	/**
	 * @brief コンストラクタ。
	 */
	GameManager() {};
	/**
	 * @brief デストラクタ。
	 */
	~GameManager() {};


	/**
	 * @brief 初期化時に呼ばれる開始処理。
	 * @return 成功した場合は true。
	 */
	bool Start();
	/**
	 * @brief 毎フレームの更新処理。
	 */
	void Update();


public:
	/**
	 * @brief 現在のシーンを取得します。
	 * @return 現在のシーン状態。
	 */
	const SceneState GetScene() const
	{
		return m_currentScene_;
	}


public:
	SceneState m_nextScene = SceneState::EnTitle; //!< 次に遷移するシーン


private:

	/**
	 * @brief シーンを切り替えます。
	 * @param createScene 生成して遷移するシーン。
	 * @param deleteScene 破棄するシーン。
	 */
	void ChangeScene(SceneState createScene, SceneState deleteScene);


	/**
	 * @brief 内部状態の初期化を行います。
	 */
	void Init();


private:
	SceneState m_currentScene_ = SceneState::EnNone; //!< 現在のシーン
	Title* m_title_ = nullptr;   //!< タイトルシーンのインスタンス
	Game* m_game_ = nullptr;     //!< ゲームシーンのインスタンス
	Result* m_result_ = nullptr; //!< リザルトシーンのインスタンス
};


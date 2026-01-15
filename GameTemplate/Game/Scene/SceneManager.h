/**
 * @file SceneManager.h
 * @brief シーン管理を行う GameManager の宣言。
 * @details タイトル/ゲーム/リザルトなどのシーン状態を保持し、
 *          遷移先の指定や現在シーンの取得を提供します。
 */
#pragma once

class IScene;

class SceneManager
{
private:
	IScene* currentScene_ = nullptr; //!< 現在のシーン
	float elapsedTime_ = 0.0f;		 //!< 経過時間
	float waitTime_ = 0.0f;			 //!< 待機時間

private:
	/* シングルトン */
	SceneManager();
	/* デストラクタ */
	~SceneManager();


public:
	/** 初期化 */
	bool Start();
	/** 更新処理 */
	void Update();
	/** 描画処理 */
	void Render(RenderContext& rc);

	IScene* GetCorrentScene() { return currentScene_; }
private:
	IScene* NextScene(const uint32_t nextId);


	/************************* シングルトン **********************************/
private:
	static SceneManager* instance_;//!< インスタンス


public:
	/* インスタンスを作成 */
	static void CreateInstance()
	{
		if (instance_ == nullptr) {
			instance_ = new SceneManager();
		}
	}

	/* 利用可能状態か確認する関数 */
	static bool IsAvailable()
	{
		return instance_ != nullptr;
	}
	/**
	 * @brief インスタンスを入手
	 * @return
	 */
	static SceneManager& Get()
	{
		return *instance_;
	}

	/**
	 * @brief インスタンスを削除
	 */
	static void DeleteInstance()
	{
		if (instance_)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}
};


class SceneManagerObject : public IGameObject
{
public:
	/* シングルトン */
	SceneManagerObject();
	/* デストラクタ */
	~SceneManagerObject();

	/** 初期化 */
	bool Start();
	/** 更新処理 */
	void Update();
	/** 描画処理 */
	void Render(RenderContext& rc);
};

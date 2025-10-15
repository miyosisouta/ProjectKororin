/**
 * @file Title.h
 * @brief タイトルシーンを表すクラスの宣言。
 */
#pragma once


class GameManager;

class Title : public IGameObject
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	Title(){}
	/**
	 * @brief デストラクタ。
	 */
	~Title() {};


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


private:
	/**
	 * @brief ゲームシーンへの遷移を開始します。
	 */
	void LoadGameScene();


private:
	SpriteRender m_titleRender_; //!< タイトル画面用のスプライト描画器
	GameManager* m_gameManager_ = nullptr; //!< シーン遷移管理用の参照
};


/**
 * @file Result.h
 * @brief リザルトシーンを表すクラスの宣言。
 */
#pragma once


class Result : public IGameObject
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	Result() {}
	/**
	 * @brief デストラクタ。
	 */
	~Result() {}


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

};


/**
 * @file Result.h
 * @brief リザルトシーンを表すクラスの宣言。
 */
#pragma once
#include "Scene/IScene.h"

class ResultScene : public IScene
{
	appScene(ResultScene);


public:
	/**
	 * @brief コンストラクタ。
	 */
	ResultScene();
	/**
	 * @brief デストラクタ。
	 */
	virtual ~ResultScene();


	/** 初期化処理 */
	virtual bool Start()override;
	/** 更新処理 */
	virtual void Update()override;
	/** 描画処理 */
	virtual void Render(RenderContext& rc)override;

	/** 次のシーンを要求する。idは次のシーンのIDを返す。trueならば要求成功、falseならば失敗 */
	virtual bool RequestID(uint32_t& id, float& waitTime) override;


private:
	SpriteRender resultSprite_;
	bool isNextScene_ = false;
};


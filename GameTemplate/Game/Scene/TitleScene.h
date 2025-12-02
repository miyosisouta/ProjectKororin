/**
 * @file Title.h
 * @brief タイトルシーンを表すクラスの宣言。
 */
#pragma once
#include "IScene.h"


class TitleInputSyste;
class Sphere;


enum EnTitleGroundKind 
{
	EnTitleGroundKind_P,
	EnTitleGroundKind_L,
	EnTitleGroundKind_A,
	EnTitleGroundKind_Y,
	EnTitleGroundKind_Max
};


class TitleScene : public IScene
{
	appScene(TitleScene);


public:
	/**
	 * @brief コンストラクタ。
	 */
	TitleScene();
	/**
	 * @brief デストラクタ。
	 */
	virtual ~TitleScene();


	/** 初期化処理 */
	virtual bool Start()override;
	/** 更新処理 */
	virtual void Update()override;
	/** 描画処理 */
	virtual void Render(RenderContext& rc)override;

	virtual bool RequestID(uint32_t& nextID) override;


private:
	SpriteRender titleGameNameSprite_; //!< タイトル画面用のスプライト描画器
	ModelRender titleGround_[EnTitleGroundKind_Max]; //!< タイトル時の地面
	SkyCube* skyCube_ = nullptr;
	Sphere* sphere_ = nullptr;
	TitleInputSyste* inputSystem_ = nullptr;

	bool isPlayAnimation = false;				//!< シーン切り替えの演出を再生する
	float elapsedTime_ = 0.0f;					//!< 経過時間

private:
	bool isNextScene_ = false;
};


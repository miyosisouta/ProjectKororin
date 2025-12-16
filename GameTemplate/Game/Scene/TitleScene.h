/**
 * @file Title.h
 * @brief タイトルシーンを表すクラスの宣言。
 */
#pragma once
#include "IScene.h"


class TitleInputSyste;
class Sphere;
class UICanvas;
class UIIcon;


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

	virtual bool RequestID(uint32_t& nextID, float& waitTime) override;


private:
	SpriteRender gameTitleNameSprite_; //!< ゲームタイトル

	Sphere* sphere_ = nullptr;
	TitleInputSyste* inputSystem_ = nullptr;
	CalcLerpValue calcTime_;

	UICanvas* pressButtonCanvas_ = nullptr;
	UICanvas* titleGameNameCanvas_ = nullptr;
	UIIcon* icon_ = nullptr;

	bool isPlayAnimation = false;				//!< シーン切り替えの演出を再生する
	float elapsedTime_ = 0.0f;					//!< 経過時間

private:
	bool isNextScene_ = false;
};


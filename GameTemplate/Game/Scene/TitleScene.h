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


class TitleScene : public IScene
{
	appScene(TitleScene);


public:
	/* コンストラクタ */
	TitleScene();
	/* デストラクタ */
	virtual ~TitleScene();

	/** 初期化処理 */
	virtual bool Start()override;
	/** 更新処理 */
	virtual void Update()override;
	/** 描画処理 */
	virtual void Render(RenderContext& rc)override;

	/* シーン切り替え */
	virtual bool RequestID(uint32_t& nextID, float& waitTime) override;


private:
	SpriteRender gameTitleNameSprite_; //!< ゲームタイトル

	Sphere* sphere_ = nullptr;					//!< 塊
	TitleInputSyste* inputSystem_ = nullptr;	//!< タイトル操作
	CalcLerpValue calcTime_;					//!< 時間計算

	UICanvas* pressButtonCanvas_ = nullptr;		//!< ボタン画像
	UICanvas* titleGameNameCanvas_ = nullptr;	//!< ゲーム名
	UIIcon* icon_ = nullptr;					//!< ボタンアニメーション

	bool isPlayAnimation = false;	//!< シーン切り替えの演出を再生する
	float elapsedTime_ = 0.0f;		//!< 経過時間

private:
	bool isNextScene_ = false;		//!< 次のシーンに移るかのフラグ
};


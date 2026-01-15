#pragma once
#include "UI/BlackOverRay.h"

class Sphere;
class SphereSizeText : public BlackOverRay
{
public:
	SphereSizeText();
	~SphereSizeText();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

public:
	/* 塊の大きさを設定 */
	inline void SetSphereRadiusUI(const float radius) { sphereRadius_ = radius; }

private:
	/* 丸い画像の回転処理 */
	void Rotation();

	/* 丸い画像の大きさ処理 */
	void AdjustOrbScaleByRadius();

	/* 丸い画像を塊と目標の大きさとの比率をもとに大きくする処理 */
	void ApplyScaleToUIElement();

	/* 塊の目標サイズを表示するための初期化 */
	void SphereGoalSizeText();




private:
	SpriteRender textBackSprite_;			//!< UIをみやすくするための黒い背景
	SpriteRender textGreenOrbSprite_;		//!< 緑の丸い画像
	SpriteRender textBlueOrbSprite_;		//!< 青の丸い画像
	SpriteRender textPinkOrbSprite_;		//!< ピンクの丸い画像
	SpriteRender indicatorSizeSprite_;		//!< 塊の現在のサイズの指標
	SpriteRender GoalSizeOverRaySprite_;	//!< 塊の目標サイズを見やすくするための背景
	SpriteRender GoalSizeColorCornSprite_;	//!< 塊の目標サイズのUI
	FontRender sphereCurrentSizeText_;		//!< 塊の現在の大きさを表示する
	FontRender sphereGoalSizeText_;			//!< 塊の目標の大きさを表示する
	Quaternion rotationBuleSprite_ = Quaternion::Identity;		//!< 丸い画像を回転させる
	Quaternion rotationPinkSprite_ = Quaternion::Identity;		//!< 丸いピンクの画像を回転させる
	Quaternion rotationGreenSprite_ = Quaternion::Identity;		//!< 丸い画像を回転させる

private:
	float sphereRadius_ = 15.0f; //!< UI用塊の大きさ
};


#pragma once
/**
 * @brief UIシステム全体の統括と外部との接点を管理
 */

class BlackOverRay;
class ObjectView;
class SphereSizeText;
class Timer;
class Canvas : public IGameObject
{
public:
	/* コンストラクタ */
	Canvas();
	/* デストラクタ */
	~Canvas();

	/* スタート処理 */
	bool Start()override;
	/* 更新処理 */
	void Update()override;
	/* 描画処理 */
	void Render(RenderContext& rc)override;


public:
	/* 現在の時間を設定 */
	void SetTimer(const float timer);
	/* 制限時間の設定 */
	void SetLimitTimer(const float timer);

private:
	BlackOverRay* blackOverRay_ = nullptr;		//!< ブラックオーバーレイ
	ObjectView* objectView_ = nullptr;			//!< オブジェクトビュー
	SphereSizeText* sphereSizeText_ = nullptr;	//!< スフィアサイズテキスト
	Timer* timer_ = nullptr;					//!< タイマー

};


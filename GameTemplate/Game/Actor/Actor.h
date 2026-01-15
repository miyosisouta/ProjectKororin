/**
 * Actor.h
 * 見た目が存在するオブジェクトに継承させる
 */
#pragma once


class Actor : public IGameObject
{
protected:
	ModelRender modelRender_; //!< モデルレンダー
	Transform transform_;	  //!< トランスフォーム


public:
	/* コンストラクタ */
	Actor() {}
	/* デストラクタ */
	virtual ~Actor() {}

	/* スタート処理 */
	virtual bool Start() override { return true; }
	/* 更新処理 */
	virtual void Update() override {}
	/* 描画処理 */
	virtual void Render(RenderContext& rc) override {}


public:
	/* モデルの取得 */
	ModelRender& GetModelRender() { return modelRender_; }

	/* トランスフォームの取得 */
	Transform* GetTransform() { return &transform_; }
};
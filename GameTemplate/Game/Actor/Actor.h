/**
 * Actor.h
 * 見た目が存在するオブジェクトに継承させる
 */
#pragma once


class Actor : public IGameObject
{
protected:
	ModelRender modelRender_;
	Transform transform_;


public:
	/**
	 * @brief コンストラクタ
	 */
	Actor() {}
	/**
	 * @brief デストラクタ
	 */
	virtual ~Actor() {}

	virtual bool Start() override { return true; }
	virtual void Update() override {}
	virtual void Render(RenderContext& rc) override {}


public:
	Transform* GetTransform() { return &transform_; }
};
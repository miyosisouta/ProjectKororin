#pragma once


class CameraBase : public IGameObject
{
protected:
	Transform transform_;


public:
	/**
	 * @brief コンストラクタ
	 */
	CameraBase() {};
	/**
	 * @brief デストラクタ
	 */
	virtual ~CameraBase() {};


	/**
	 * @brief 初期化処理
	 * @return 
	 */
	virtual bool Start() { return true; };
	/**
	 * @brief 更新処理
	 */
	virtual void Update()override {};
	/**
	 * @brief 描画処理
	 * @param rc 
	 */
	virtual void Render(RenderContext& rc)override {};



private:

};
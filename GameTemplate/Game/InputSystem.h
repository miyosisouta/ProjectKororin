/**
 * InputSystem.h
 * 入力操作についてまとめるクラスの宣言
 */
#pragma once


class Sphere;
class InputSystem : public IGameObject
{
protected:
	Sphere* target_ = nullptr;
	int inputNo_ = 0;


public:
	/**
	 * @brief コンストラクタ
	 */
	InputSystem() {}
	/**
	 * @brief デストラクタ
	 */
	~InputSystem() {}

	/**
	 * @brief オブジェクトの開始処理を実行します。
	 * @return 開始処理が成功した場合は true、失敗した場合は false を返します。
	 */
	bool Start() override;
	/**
	 * @brief オブジェクトの状態を更新します。基底クラスのUpdateメソッドをオーバーライドしています。
	 */
	void Update() override;
	/**
	 * @brief 描画処理を実行します
	 * @param rc 
	 */
	void Render(RenderContext& rc) override;


public:
	/**
	 * @brief ターゲットとなる Sphere オブジェクトを設定します。
	 * @param target 設定する Sphere オブジェクトへのポインタ。
	 */
	void SetTarget(Sphere* target) { target_ = target; }


	/**
	 * @brief コントローラー番号を設定します。
	 * @param no 設定するコントローラー番号。
	 */
	void SetControllerNo(const int no) { inputNo_ = no; }


private:
	/**
	 * @brief 指定された入力番号に対応するパッドを取得します。
	 * @return 入力番号に対応するパッドへのポインタ。
	 */
	auto* GetPad() { return g_pad[inputNo_]; }


	/**
	 * @brief 左スティックの現在の位置を取得します。
	 * @return 左スティックの移動速度を表す。
	 */
	Vector3 GetStickL();
};




class TitleInputSyste : public InputSystem
{
private:
	/** 移動方向 */
	Vector3 moveDirection_;


public:
	TitleInputSyste() {}
	~TitleInputSyste() {}

	/**
	 * @brief オブジェクトの開始処理を実行します。
	 * @return 開始処理が成功した場合は true、失敗した場合は false を返します。
	 */
	bool Start() override;
	/**
	 * @brief オブジェクトの状態を更新します。基底クラスのUpdateメソッドをオーバーライドしています。
	 */
	void Update() override;
	/**
	 * @brief 描画処理を実行します
	 * @param rc
	 */
	void Render(RenderContext& rc) override {}


public:
	void SetMoveDirection(const Vector3& direction) { moveDirection_ = direction; }
};
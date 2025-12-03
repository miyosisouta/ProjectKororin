/* 
 * # : 文字を数字に置き換える
 * \ : 改行
 * 文字列の場合比較に時間がかかるため数字に変換する
 */
#pragma once
#define appScene(name)\
public:\
	static constexpr uint32_t ID() { return appHash32(#name); }


class IScene
{
public:
	IScene() {}
	virtual ~IScene() {}


	/**	開始処理 **/
	virtual bool Start() = 0;
	/** 通常更新 */
	virtual void Update() = 0;
	/** 描画処理 */
	virtual void Render(RenderContext& rc) = 0;

	/**	次のシーンを要求する。idは次のシーンのIDを返す。trueならば要求成功、falseならば失敗 */
	virtual bool RequestID(uint32_t& id, float& waitTime) = 0;

	void ButtonUI(SpriteRender& spritePath, FontRender& font, Vector3 spritePos, Vector3 fontPos);
};
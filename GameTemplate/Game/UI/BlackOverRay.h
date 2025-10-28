#pragma once
/**
 * @\brief 共通のインターフェースを定義する役割
 */


class BlackOverRay : public IGameObject
{
public:
	/* コンストラクタ */
	BlackOverRay() {};
	/* デストラクタ */
	virtual ~BlackOverRay() {}

	//*< IGameObjectにてStart、Update、Renderを仮想関数にしているため
	//*< 継承元は絶対にStart、Update、Renderはヘッダーにて書きoverrideをかく
	//*< またこのクラスも継承されているのでvirtualをかく
	virtual bool Start() override { return true; }
	virtual void Update() override {}
	virtual void Render(RenderContext& rc) override {}



protected:
	FontRender fontRender_;

	/**
	 * @brief コンパイル時にUIのファイルパスを定数化
	 */
	static constexpr const char* UIAseetsFilePath = "Assets/Sprite/UI/";


protected:

	/**
	 * @brief 黒色のディスプレイを表示
	 * @param spriteRender_ 
	 */
	void RenderDarkOverlay(SpriteRender* spriteRender_, Vector3 pos);


	/**
	 * @brief UIの画像の初期化・Update
	 * @param spriteRender_ スプライトレンダー
	 * @param failPath ファイルパス
	 * @param pos 座標
	 * @param scale 大きさ
	 * @param mulColor 乗算カラー
	 */
	void Initialize(SpriteRender* spriteRender, const std::string failPath, const int pixelSize ,const Vector3 pos, const Vector3 scale, const Vector4 mulColor);


	/**
	 * @brief UIのアセット用ファイルパスを見つける関数
	 * @param spriteName 使いたいddsファイルの名前
	 * @return 
	 */
	inline std::string FindFailPath_Sprite(const std::string& spriteName)
	{
		return std::string(UIAseetsFilePath) + spriteName;
	}


};


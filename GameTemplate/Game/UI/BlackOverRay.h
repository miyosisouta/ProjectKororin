#pragma once
/**
 * @\brief 共通のインターフェースを定義する役割
 */

/**
 * @brief 画像をInitializeするための情報を共有する役割
 */
namespace SpriteConstans
{
	// スプライトに必要な情報
	struct SpriteRenderInfo
	{
		// スプライトに必要な情報を設定 : コンストラクタなのでconstでも複数の値を設定できる
		// ※定義時に必ず値を渡すこと
		SpriteRenderInfo(const std::string path, const int s, const Vector3& p, const Vector3& scal, const Vector4& c)
			: filePath(path)
			, size(s)
			, pos(p)
			, scale(scal)
			, color(c)
		{
		}

		std::string filePath; // ファイルパス
		int size; // ピクセルサイズ
		Vector3 pos; // 座標
		Vector3 scale; // 画像サイズの倍率
		Vector4 color; // 乗算カラー
	};
}


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
	/**
	 * @brief コンパイル時にUIのファイルパスを定数化
	 */
	static constexpr const char* UIAseetsFilePath = "Assets/Sprite/UI/";


protected:

	/**
	 * @brief 黒色のディスプレイを表示
	 * @param spriteRender_ 
	 */
	void RenderDarkOverlay(SpriteRender* spriteRender_, Vector3 pos, Vector3 scale);


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


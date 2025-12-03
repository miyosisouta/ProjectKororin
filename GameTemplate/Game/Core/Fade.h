/**
 * Fade.h
 * ゲームの時間を管理するクラス
 */
#pragma once

/* 現在のフェード状態 */
enum class FadeMode 
{
    FadeOut,        //!< 明るくなる
    FadeIn,         //!< 暗くなる
    FadeMode_Max,    //!< モード数
    None           //!< 何もない

};

/* 色の事前設定 */
namespace fadeColorPreset
{
    static const Vector4 WHITE_COLOR_INVISIBLE = Vector4(1.0f, 1.0f, 1.0f, 0.0f);		//!< 白色、透明
    static const Vector4 BLACK_COLOR_INVISIBLE = Vector4(0.0f, 0.0f, 0.0f, 0.0f);		//!< 黒色、透明
    static const Vector4 WHITE_COLOR = Vector4(1.0f, 1.0f, 1.0f, 1.0f);		            //!< 白色
    static const Vector4 BLACK_COLOR = Vector4(0.0f, 0.0f, 0.0f, 1.0f);		            //!< 黒色
    static const Vector3 WHITE_COLOR_RGB = Vector3(1.0f, 1.0f, 1.0f);				    //!< 白色、Vec3
    static const Vector3 BLACK_COLOR_RGB = Vector3(0.0f, 0.0f, 0.0f);			    	//!< 黒色、Vec3
}

class Fade
{
private:
    FadeMode mode_ = FadeMode::FadeMode_Max; //!< 現在のフェード状態

    SpriteRender fadeSprite_;           //!< フェード用画像
    SpriteRender loadingStaticSprite_;  //!< 「 NowLoading 」画像
    SpriteRender loadingMoveSprite_;    //!< ローディングで動く画像

    Vector3 currentFadeColor_ = fadeColorPreset::WHITE_COLOR_RGB; // 現在の色
    Vector3 currentLoadingColor_ = fadeColorPreset::WHITE_COLOR_RGB; // 現在の色
    bool isPlay_ = false;       //!< フェード画像の描画をするかどうか
    bool isLoading_ = false;    //!< ローディング画面の描画をするかどうか
    float fadeOutAlpha_ = 0.0f;             //!< 最初は透明に
    float elapsedTime_ = 0.0f;	            //!< 経過時間
    float fadeTime_ = 0.0f;                 //!< フェードをする時間
    

private:
    Fade();
    ~Fade();


public:
    void Update();
    void Render(RenderContext& rc);

    /**
     * @brief フェードを始める前の設定
     * @param mode フェードインかアウトか  デフォルトはフェードアウト
     * @param fadeTime フェード時間の設定　デフォルトは2.0f
     * @param color 画像の色の設定         デフォルトは白の透明
     */
    void PlayFade(FadeMode mode = FadeMode::FadeOut, float fadeTime = 2.0f, Vector3 color = fadeColorPreset::WHITE_COLOR_RGB);

private:
    /** @brief 画像のα値の更新 */
    void UpdateAlpha();

    /** ローディングの更新 */
    void UpdateLoading();


public:
    inline void SetAlpha(const float alpha) { fadeOutAlpha_ = alpha; }  //!< α値の設定
    inline void SetLoading(const bool flag) { isLoading_ = flag; }      //!< ローディング画面を描画するかどうか(trueに設定するだけ)
    inline void Play() { isPlay_ = true; }                              //!< フェードさせるかどうか(trueに設定するだけ)
    inline void Stop() { isPlay_ = false; }                             //!< フェード停止
    inline bool IsLoading() { return isLoading_; }                      //!< ローディング画面を描画させているどうか
    inline bool IsPlay() { return isPlay_; }                            //!< フェードしているかどうか


/******* シングルトン用 **********/

private:
    static Fade* instance_;


public:
    /**
     * インスタンスを作る
     */
    static void CreateInstance()
    {
        if (instance_ == nullptr)
        {
            instance_ = new Fade();
        }
    }

    /// <summary>
    /// インスタンスを取得
    /// </summary>
    static Fade& Get()
    {
        return *instance_;
    }

    /// <summary>
    /// インスタンスを破棄
    /// </summary>
    static void DestroyInstance()
    {
        if (instance_ != nullptr)
        {
            delete instance_;
            instance_ = nullptr;
        }
    }
};

class FadeObject : public IGameObject
{
public:
    FadeObject();
    ~FadeObject();

    bool Start() { return true; }
    void Update();
    void Render(RenderContext& rc);
};
 
   
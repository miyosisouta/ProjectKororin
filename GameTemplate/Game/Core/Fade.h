/**
 * Fade.h
 * ゲームの時間を管理するクラス
 */
#pragma once

class Fade
{
private:
    SpriteRender fadeOutSprite_;
    float fadeColor_ = 1.0f;    // 最初は白に
    float fadeOutAlpha_ = 0.0f; // 最初は透明に


private:
    Fade();
    ~Fade();


public:
    void Update();
    void Render(RenderContext& rc);


    inline void SetAlpha(const float alpha) { fadeOutAlpha_ = alpha; } // α値の設定
    inline void SetColor(const float color) { fadeColor_ = color; } // 色の設定　0.0f : 黒 1.0f : 白


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
 
   
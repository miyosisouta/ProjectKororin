/**
 * Fade.h
 * ゲームの時間を管理するクラス
 */
#pragma once

class Fade
{
private:
    SpriteRender whiteOutSprite_;
    float whiteOutAlpha_;


private:
    Fade();
    ~Fade();


public:
    void Update();
    void Render(RenderContext& rc);


    inline void SetAlpha(const float alpha) { whiteOutAlpha_ = alpha; }



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
 
   
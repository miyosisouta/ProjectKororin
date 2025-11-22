/**
 * GameTimer.h
 * ゲームの時間を管理するクラス
 */
#pragma once

/**
 * GameTimer.h
 * ゲームの制限時間を計算するクラス
 */
class GameTimer
{
private:
    /** 残り時間 */
    float remainingTime_ = 0.0f;
    /** ゲームの時間(最初に設定されるだけ) */
    float gameTime_ = 0.0f;


private:
    GameTimer();
    ~GameTimer();


public:
    void Update();


public:
    inline float GetRemainingTime() { return remainingTime_; }
    inline float GetGameTime() { return gameTime_; }
    inline void SetGameTime(const float time) { gameTime_ = time; }
    inline void Init() { remainingTime_ = gameTime_; };



/**
 * シングルトン用
 */
private:
    static GameTimer* instance_;


public:
    /**
     * インスタンスを作る
     */
    static void CreateInstance()
    {
        if (instance_ == nullptr)
        {
            instance_ = new GameTimer();
        }
    }

    /// <summary>
    /// インスタンスを取得
    /// </summary>
    static GameTimer& Get()
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
 
   
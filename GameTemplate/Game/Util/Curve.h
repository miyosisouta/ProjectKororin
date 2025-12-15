#pragma once

// 汎用的なclamp関数テンプレート
template <typename T>
T clamp(T value, T low, T high) {
    if (value < low) {
        return low;
    }
    if (value > high) {
        return high;
    }
    return value;
}


/**
 * @brief イージングとループの種類
 */
enum class EasingType { Linear, EaseIn, EaseOut, EaseInOut }; // 線形補間、イーズイン、イーズアウト、イーズインアウト
enum class LoopMode { Once, Loop, PingPong }; // 片道、周回(上から下、上から下を繰り返す)、往復(上から下、下から上を繰り返す)


template <typename T>
class Curve
{
private:
    T _startValue;          //!< 始める数値
    T _endValue;            //!< 終わる数値
    float _duration;        //!< 時間の間隔
    float _currentTime;     //!< 現在の時間
    EasingType _easingType; //!< イージングタイプ
    LoopMode _loopMode;     //!< ループモード
    bool _isPlaying;        //!< 再生するか
    int _direction;         //!< 方向

    /* イージングタイプごとの時間の計算ロジック（ここは float のままでOK）*/
    float ApplyEasingInternal(float t) const {
        switch (_easingType) {
        case EasingType::Linear:    return t;
        case EasingType::EaseIn:    return t * t;
        case EasingType::EaseOut:   return t * (2.0f - t);
        case EasingType::EaseInOut:
            if (t < 0.5f) return 2.0f * t * t;
            else          return -1.0f + (4.0f - 2.0f * t) * t;
        default: return t;
        }
    }

public:
    Curve() : _currentTime(0), _duration(1.0f), _isPlaying(false), _direction(1) {}

    /* 初期化 */
    void Play(T start, T end, float timeSec, EasingType type = EasingType::EaseInOut, LoopMode loopMode = LoopMode::Once) {
        _startValue = start;
        _endValue = end;
        _duration = max(0.0001f, timeSec);
        _easingType = type;
        _loopMode = loopMode;
        _currentTime = 0.0f;
        _isPlaying = true;
        _direction = 1;
    }

    /* 更新処理(前回と全く同じ) */
    void Update(float deltaTime) {
        if (!_isPlaying) return;

        if (_loopMode == LoopMode::PingPong) _currentTime += deltaTime * _direction;
        else _currentTime += deltaTime;

        if (_currentTime >= _duration) {
            if (_loopMode == LoopMode::Once) {
                _currentTime = _duration;
                _isPlaying = false;
            }
            else if (_loopMode == LoopMode::Loop) {
                _currentTime = 0.0f;
            }
            else if (_loopMode == LoopMode::PingPong) {
                _currentTime = _duration;
                _direction = -1;
            }
        }
        else if (_currentTime <= 0.0f) {
            if (_loopMode == LoopMode::PingPong) {
                _currentTime = 0.0f;
                _direction = 1;
            }
        }
    }

    /* 現在の値を取得 */
    T GetCurrentValue() const {
        float t = clamp<float>(_currentTime / _duration, 0.0f, 1.0f); //!< 時間によるクランプ処理
        float rate = ApplyEasingInternal(t); //!< イージングタイプごとの時間の設定

        //!< 線形補間 (Lerp)
        //!< 注意: T型(Vector3等)は、演算子 +, -, * (float) を持っている必要があります
        return _startValue + (_endValue - _startValue) * rate;
    }

    bool IsPlaying() const { return _isPlaying; } // 現在再生中かどうか
};

/*
 * 名前ごとに型を設定
 * Curve<template> : template :何の型でも設定可能
 * そのため使う型ごとに名前を設定
 */
using Vector2Curve = Curve<Vector2>;
using Vector3Curve = Curve<Vector3>;
using Vector4Curve = Curve<Vector4>;
using QuaternionCurve = Curve<Quaternion>;
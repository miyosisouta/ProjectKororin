/**
 * 汎用的な処理のまとめ
 */
#pragma once

/**
 * @brief Lerpなどで使用するパーセントを計算
 */
class CalcLerpValue : public Noncopyable
{
private:
	float elapsedTime_ = 0.0f;	// 経過時間
	float targetTime_ = 0.0f;	// 目標時間


public:
	/* 時間の初期化 */
	/* 初期は1.0f */
	void InitCalcTime(const float time = 1.0f) 
	{
		targetTime_ = time;
		elapsedTime_ = 0.0f;
	}

	/* 目標時間になるまで毎フレーム時間を計算、初期は最大1.0f*/
	float CalcUpdate(const float ratio = 1.0f)
	{
		elapsedTime_ += g_gameTime->GetFrameDeltaTime();

		return min(ratio, (elapsedTime_ / targetTime_));
	}
};
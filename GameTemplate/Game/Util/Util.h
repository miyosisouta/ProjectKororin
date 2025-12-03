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
	void SetTargetTime(const float time) { targetTime_ = time; }

	float CalcUpdate()
	{
		elapsedTime_ += g_gameTime->GetFrameDeltaTime();

		return min(1.0f, (elapsedTime_ / targetTime_));
	}
};
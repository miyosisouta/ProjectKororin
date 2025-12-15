#pragma once
/**
 * Input.h
 * ボタン入力を管理するクラス
 */
class InputDetection
{
private:
	bool isCurrentTriggerButtonA_ = false;	//!< Aボタンを現フレームで押したかどうか
	bool isOldTiggerButtonA_ = false;		//!< Aボタンを前フレームで押したかどうか

public:
	/* 押したかどうかの状態の更新処理 */
	void UpdateTriggerState();

	/* Aボタンを押したかつ、前フレームは押していない状態かどうか */
	bool IsTriggerButtonA();
};


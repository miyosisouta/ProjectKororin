/**
 * Types.h
 * サウンド用の定数など必要な情報を定義するファイル
 */
#pragma once
#include <string>


 /** サウンドの種類 */
enum enSoundKind
{
	enSoundKind_SE = 0,
	enSoundKind_Attach_Tiny = enSoundKind_SE, //!< 1番小さいオブジェクトの吸着音
	/* ここからSEを追加 */
	enSoundKind_Attach_Small,				//!< 2番目の吸着音
	enSoundKind_Attach_Medium,				//!< 3番目の吸着音
	enSoundKind_Attach_Large,				//!< 4番目の吸着音
	enSoundKind_AttachSuccess,				//!< 吸着成功時
	enSoundKind_AttachFailure,				//!< 吸着失敗時
	enSoundKind_Decision,					//!< 決定時の音
	enSoundKind_RateStamp,					//!< 評価スタンプの音
	enSoundKind_SE_Max,						//!< SEの最大数
	enSoundKind_BGM = enSoundKind_SE_Max,
	enSoundKind_Title = enSoundKind_BGM,
	/* ここからBGMを追加 */
	enSoundKind_Game,
	enSoundKind_ResultSuccess,				//!< 目標のサイズを達成しているとき
	enSoundKind_ResultFailureSound,			//!< 目標のサイズを達成していないとき
	enSoundKind_Max,						//!< サウンドの最大数
	enSoundKind_None = enSoundKind_Max,
};


/** サウンドの情報の構造体 */
struct SoundInformation
{
	std::string assetPath;
	//
	SoundInformation(const std::string& path) : assetPath(path) {}
};


/**
 *	情報を保持
 *	流すsoundファイルをSoundInformationに格納
 */
static SoundInformation soundInformation[enSoundKind_SE_Max] =
{
	// SE
	SoundInformation("Assets/Audio/SE/Attach_Tiny.wav"),	//!< 1番小さいSE	
	SoundInformation("Assets/Audio/SE/Attach_Small.wav"),	//!< 2番目に小さいSE
	SoundInformation("Assets/Audio/SE/Attach_Medium.wav"),	//!< 2番目に大きいSE
	SoundInformation("Assets/Audio/SE/Attach_Large.wav"),	//!< 1番大きいSE
	SoundInformation("Assets/Audio/SE/AttachSuccess.wav"),	//!< テスト用SE
	SoundInformation("Assets/Audio/SE/AttachFailure.wav"),	//!< 吸着不可SE
	SoundInformation("Assets/Audio/SE/decision.wav"),		//!< 決定ボタン
	SoundInformation("Assets/Audio/SE/RateStamp.wav")		//!< スタンプ音


	////BGM
	//SoundInformation("Assets/Audio/BGM/TitleSound.wav"),
	//SoundInformation("Assets/Audio/BGM/GameSound.wav"),
	//SoundInformation("Assets/Audio/BGM/ResultSuccess.wav"),
	//SoundInformation("Assets/Audio/BGM/ResultFailureSound.wav"),


};


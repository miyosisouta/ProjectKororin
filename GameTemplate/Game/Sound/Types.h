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
	enSoundKind_AttachSuccess = enSoundKind_SE, // 吸着成功時
	/* ここからSEを追加 */
	enSoundKind_AttachFailure, // 吸着失敗時
	enSoundKind_SE_Max,	// SEの最大数
	enSoundKind_BGM = enSoundKind_SE_Max,
	enSoundKind_Title = enSoundKind_BGM,
	/* ここからBGMを追加 */
	enSoundKind_Game,
	enSoundKind_ResultSuccess,
	enSoundKind_ResultFailureSound,
	enSoundKind_Max, // サウンドの最大数
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
	SoundInformation("Assets/Audio/SE/AttachSuccess.wav"),
	SoundInformation("Assets/Audio/SE/AttachFailure.wav"),

	////BGM
	//SoundInformation("Assets/Audio/BGM/TitleSound.wav"),
	//SoundInformation("Assets/Audio/BGM/GameSound.wav"),
	//SoundInformation("Assets/Audio/BGM/ResultSuccess.wav"),
	//SoundInformation("Assets/Audio/BGM/ResultFailureSound.wav"),


};


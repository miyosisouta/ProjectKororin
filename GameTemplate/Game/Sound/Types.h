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
	enSoundKind_Attach = enSoundKind_SE, // 吸着時のSE
	/* ここからSEを追加 */
	enSoundKind_SE_Max,	// SEの最大数
	enSoundKind_BGM = enSoundKind_SE_Max,
	enSoundKind_Game = enSoundKind_BGM,
	/* ここからBGMを追加 */
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
	SoundInformation("Assets/Audio/SE/AttachSound.wav"),
};


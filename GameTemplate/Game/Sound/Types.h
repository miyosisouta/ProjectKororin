/**
 * Types.h
 * �T�E���h�p�̒萔�ȂǕK�v�ȏ����`����t�@�C��
 */
#pragma once
#include <string>


 /** �T�E���h�̎�� */
enum enSoundKind
{
	enSoundKind_SE = 0,
	enSoundKind_AttachSuccess = enSoundKind_SE, // �z��������
	/* ��������SE��ǉ� */
	enSoundKind_AttachFailure, // �z�����s��
	enSoundKind_SE_Max,	// SE�̍ő吔
	enSoundKind_BGM = enSoundKind_SE_Max,
	enSoundKind_Title = enSoundKind_BGM,
	/* ��������BGM��ǉ� */
	enSoundKind_Game,
	enSoundKind_ResultSuccess,
	enSoundKind_ResultFailureSound,
	enSoundKind_Max, // �T�E���h�̍ő吔
	enSoundKind_None = enSoundKind_Max,
};


/** �T�E���h�̏��̍\���� */
struct SoundInformation
{
	std::string assetPath;
	//
	SoundInformation(const std::string& path) : assetPath(path) {}
};


/** 
 *	����ێ� 
 *	����sound�t�@�C����SoundInformation�Ɋi�[
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


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
	enSoundKind_Attach = enSoundKind_SE, // �z������SE
	/* ��������SE��ǉ� */
	enSoundKind_SE_Max,	// SE�̍ő吔
	enSoundKind_BGM = enSoundKind_SE_Max,
	enSoundKind_Game = enSoundKind_BGM,
	/* ��������BGM��ǉ� */
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
	SoundInformation("Assets/Audio/SE/AttachSound.wav"),
};


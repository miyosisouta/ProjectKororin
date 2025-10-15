/**
 * SoundManager.cpp
 * �T�E���h�Ǘ�
 * �K�v�ȃT�E���h�t�@�C����ǂݍ��񂾂�Đ�������ȂǊǗ�����
 */
#include "stdafx.h"
#include "SoundManager.h"


SoundManager* SoundManager::instance_ = nullptr; //������


SoundManager::SoundManager()
{
	seList_.clear();

	// �T�E���h�̓o�^
	for (int i = 0; i < ARRAYSIZE(soundInformation); ++i) {
		const auto& info = soundInformation[i];
		g_soundEngine->ResistWaveFileBank(i, info.assetPath.c_str());
	}
}


SoundManager::~SoundManager()
{
}


void SoundManager::Update()
{
	// SE���X�g����Đ����Ă��Ȃ����̂�����΍폜����
	std::vector<SoundHandle> eraseList;
	for (auto& it : seList_) {
		const auto key = it.first;
		auto* se = it.second;
		// �Đ����I����Ă���Ȃ�폜
		if (!se->IsPlaying()) {
			eraseList.push_back(key);
		}
	}
	for (const auto& key : eraseList) {
		seList_.erase(key);
	}
}


void SoundManager::PlayBGM(const int kind)
{
	// BGM����������Ă��Ȃ�
	if (bgm_ == nullptr) {
		// ����
		bgm_ = NewGO<SoundSource>(0, "bgm");
	}
	else {
		// ���łɐ�������Ă���Ȃ�BGM���~����
		bgm_->Stop();
	}
	// ������
	bgm_->Init(kind);
	bgm_->Play(true);	// BGM�Ȃ̂Ń��[�v�Đ�����
}


void SoundManager::StopBGM()
{
	if (bgm_ == nullptr) {
		return;
	}
	bgm_->Stop();
}


SoundHandle SoundManager::PlaySE(const int kind, const bool isLood, const bool is3D)
{
	// �n���h�����ő吔�ɂȂ�����g���Ȃ�
	// NOTE: ����ȂɍĐ�����͂����Ȃ�
	if (soundHandleCount_ == INVALID_SOUND_HANDLE) {
		K2_ASSERT(false, "�T�E���h�̍Đ��������ł��B\n");
		return INVALID_SOUND_HANDLE;
	}
	auto* se = NewGO<SoundSource>(0, "se");
	se->Init(kind, is3D);
	se->Play(isLood);

	seList_.emplace(soundHandleCount_++, se);

	return soundHandleCount_;
}


void SoundManager::StopSE(const SoundHandle handle)
{
	auto* se = FindSE(handle);
	if (se == nullptr) {
		return;
	}
	se->Stop();
}
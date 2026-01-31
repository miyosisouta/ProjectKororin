/**
 * SoundManager.cpp
 * サウンド管理
 * 必要なサウンドファイルを読み込んだり再生したりなど管理する
 */
#include "stdafx.h"
#include "SoundManager.h"

namespace {
	const constexpr float MIN_VOLUME = 0.01f;	// BGMの最小音量
	const constexpr float MAX_VOLUME = 1.0f;	// BGMの最大音量
}


SoundManager* SoundManager::instance_ = nullptr; //初期化


SoundManager::SoundManager()
{
	// リストの削除
	seList_.clear();

	// サウンドの登録
	for (int i = 0; i < ARRAYSIZE(soundInformation); ++i) {
		const auto& info = soundInformation[i];
		g_soundEngine->ResistWaveFileBank(i, info.assetPath.c_str());
	}
}

SoundManager::~SoundManager()
{
	DeleteGO(bgm_);
	DeleteGO(se_);
}


void SoundManager::Update()
{
	// SEリストから再生していないものがあれば削除する
	{
		std::vector<SoundHandle> eraseList;
		for (auto& it : seList_) {
			const auto key = it.first;
			auto* se = it.second;
			// 再生が終わっているなら削除
			if (!se->IsPlaying())
			{
				eraseList.push_back(key);
			}
		}
		for (const auto& key : eraseList) {
			seList_.erase(key);
		}
	}

	// BGMのフェードアウト処理する場合
	if (isVolumeFadeOut_) {
		elapsedTime_ += g_gameTime->GetFrameDeltaTime(); // 経過時間を加算
		float value = min(MAX_VOLUME, (elapsedTime_ / fadeTime_)); // ボリュームを計算
		float volume = MAX_VOLUME - value; // ボリュームを1.0fから減算していく
		bgm_->SetVolume(volume); // ボリュームを設定 

		// 最小音量以下になったらBGMを停止してフラグをリセット
		if (bgm_->GetVolume() <= MIN_VOLUME) {
			bgm_->Stop(); // BGM停止
			isVolumeFadeOut_ = false; // フェードアウトフラグリセット
		}
	}

	// BGMのフェードイン処理する場合
	if (isVolumeFadeIn_) {
		elapsedTime_ += g_gameTime->GetFrameDeltaTime(); // 経過時間を加算
		float volume= min(MAX_VOLUME, (elapsedTime_ / fadeTime_)); // ボリュームを計算
		bgm_->SetVolume(volume); // ボリュームを設定

		// 最小音量以下になったらBGMを停止してフラグをリセット
		if (bgm_->GetVolume() >= MAX_VOLUME) {
			isVolumeFadeIn_ = false; // フェードアウトフラグリセット
		}
	}
}


void SoundManager::PlayBGM(const int kind, bool isVolumeFadeIn, float fadeTime)
{
	// BGMが生成されていないなら生成
	if (bgm_ == nullptr) {
		bgm_ = NewGO<SoundSource>(0, "bgm");
	}
	// すでに生成されているならBGMを停止する
	else {
		bgm_->Stop();
	}
	
	// フェードインのフラグをリセット
	if (isVolumeFadeOut_ == true) { isVolumeFadeOut_ = !isVolumeFadeOut_; }
	// BGMの初期化
	bgm_->Init(kind);	

	// フェードインするならフラグを立てるだけ
	if (isVolumeFadeIn) {
		isVolumeFadeIn_ = true; // フェードインフラグを立てる
		fadeTime_ = fadeTime; // フェードイン時間をセット
		bgm_->SetVolume(MIN_VOLUME); // 最初は最低音量で再生開始
		elapsedTime_ = 0.0f; // 経過時間リセット
	}

	bgm_->Play(true);	// BGMなのでループ再生する
}


void SoundManager::StopBGM(bool isVolumeFadeOut, float fadeTime)
{
	// BGMが生成されていないなら何もしない
	if (bgm_ == nullptr) { return; }
	// フェードインのフラグをリセット
	if (isVolumeFadeIn_ == true) { isVolumeFadeIn_ = !isVolumeFadeIn_; }

	// フェードアウトするならフラグを立てるだけ
	if (isVolumeFadeOut) {
		isVolumeFadeOut_ = true; // フェードアウトフラグを立てる
		fadeTime_ = fadeTime; // フェードアウト時間をセット
		elapsedTime_ = 0.0f; // 経過時間リセット
		return;
	}

	// フェードアウトしないなら即座に停止
	bgm_->Stop();
}


SoundHandle SoundManager::PlaySE(const int kind, const bool isLood, const bool is3D)
{
	// ハンドルが最大数になったら使えない
	// NOTE: そんなに再生するはずがない
	if (soundHandleCount_ == INVALID_SOUND_HANDLE) {
		K2_ASSERT(false, "サウンドの再生が多いです。\n");
		return INVALID_SOUND_HANDLE;
	}
	se_ = NewGO<SoundSource>(0, "se");
	se_->Init(kind, is3D);
	se_->Play(isLood);

	seList_.emplace(++soundHandleCount_, se_);

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
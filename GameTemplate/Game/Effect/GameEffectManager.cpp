#include "stdafx.h"
#include "GameEffectManager.h"
#include "graphics/effect/EffectEmitter.h"


GameEffectManager* GameEffectManager::instance_ = nullptr; //インスタンス


GameEffectManager::GameEffectManager()
{
	// リストの削除
	effectList_.clear();

	// リストの登録
	for (int i = 0; i < ARRAYSIZE(effectInfo); ++i) {
		const auto& info = effectInfo[i];
		EffectEngine::GetInstance()->ResistEffect(i, info.assetPath);
	}
}

GameEffectManager::~GameEffectManager()
{
	if (effect_) {
		DeleteGO(effect_);
	}
}


EffectHandle GameEffectManager::Play(const int kind, const Vector3& pos, const Quaternion& rot, Vector3& scal)
{
	if (effectHandleCount_ == INVALID_EFFECT_HANDLE)
	{
		K2_ASSERT(false, "エフェクトの再生が多いです。\n");
		return INVALID_EFFECT_HANDLE;
	}
	effect_ = NewGO<EffectEmitter>(0);
	effect_->Init(kind); // 読み込み
	effect_->SetPosition(pos); // 座標を設定
	effect_->SetRotation(rot); // 回転を設定
	effect_->SetScale(scal);   // 大きさを設定
	effect_->Play(); // 再生

	EffectHandle handle = effectHandleCount_;
	effectList_[handle] = effect_;
	effectHandleCount_++;
	return handle;
}


void GameEffectManager::Stop(const EffectHandle handle)
{
	auto* effect = FindEffect(handle);
	if (effect == nullptr)
	{
		return;
	}
	effect->Stop();
}

/********************** インスタンスを作成するためのクラス ***********************/

GameEffectObject::~GameEffectObject()
{
	GameEffectManager::Get().DestroyInstance(); // インスタンスの破棄
}


bool GameEffectObject::Start()
{
	GameEffectManager::Get().CreateInstance(); // インスタンスの生成
	return true;
}

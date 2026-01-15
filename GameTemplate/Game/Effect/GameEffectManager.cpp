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


EffectHandle GameEffectManager::Play(const int kind, const Vector3& pos, const Quaternion& rot, Vector3& scal)
{
	if (effectHandleCount_ == INVALID_EFFECT_HANDLE)
	{
		K2_ASSERT(false, "エフェクトの再生が多いです。\n");
		return INVALID_EFFECT_HANDLE;
	}
	EffectEmitter* effect = NewGO<EffectEmitter>(0);
	effect->Init(kind);
	effect->SetPosition(pos);
	effect->SetRotation(rot);
	effect->SetScale(scal);

	return effectHandleCount_;
}


void GameEffectManager::Stop(const EffectHandle handle)
{
	auto effect = FindEffect(handle);
	if (effect_ == nullptr)
	{
		return;
	}
	effect_->Stop();
}

/*********************************************/

GameEffectObject::~GameEffectObject()
{
	GameEffectManager::Get().DestroyInstance(); // インスタンスの破棄
}


bool GameEffectObject::Start()
{
	GameEffectManager::Get().CreateInstance(); // インスタンスの生成
	return true;
}

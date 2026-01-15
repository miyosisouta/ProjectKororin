#include "stdafx.h"
#include "GameUIManager.h"
#include "UI/SphereSizeText.h"
#include "UI/ObjectView.h"


void GameUIUpdate::Update()
{
	// 更新処理
	GameUIManager::Get().Update();
}


/*****************/


GameUIManager* GameUIManager::instance_ = nullptr;


GameUIManager::GameUIManager()
{
	// リストの要素を削除
	notifyList_.clear();

}


GameUIManager::~GameUIManager()
{
	ClearNotify();
}


void GameUIManager::Update()
{
	for (auto* notify : notifyList_)
	{
		// 型をNotifySphereSizeTextに変更
		NotifySphereSizeText* sphereSizeTextNotify = static_cast<NotifySphereSizeText*>(notify);
		// Sphereの半径をUIクラスに渡す
		sphereSizeText->SetSphereRadiusUI(sphereSizeTextNotify->radius);
	}
}

void GameUIManager::Notify(UIINotify* notify)
{
	// 通知を入れる
	notifyList_.push_back(notify);
}


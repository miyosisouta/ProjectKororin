#include "stdafx.h"
#include "GameUIManager.h"
#include "UI/SphereSizeText.h"
#include "UI/ObjectView.h"


void GameUIUpdate::Update()
{
	GameUIManager::Get().Update();
}


/*****************/


GameUIManager* GameUIManager::instance_ = nullptr;


GameUIManager::GameUIManager()
{
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
		switch (notify->type)
		{
		case UINotifyType::Enum::SphereSizeText:
		{
			NotifySphereSizeText* sphereSizeTextNotify = static_cast<NotifySphereSizeText*>(notify); // 型をNotifySphereSizeTextに変更
			sphereSizeText->SetSphereRadiusUI(sphereSizeTextNotify->radius); // Sphereの半径をUIクラスに渡す

			break;
		}
		case UINotifyType::Enum::ObjectView:
		{
			NotifyObjectView* objectViewNotify = static_cast<NotifyObjectView*>(notify);

			break;

		}
		default: // それ以外の場合通知がない
		{
			K2_ASSERT(false, "通知の種類が追加されていません。\n");
			break;
		}
		}
	}
}

void GameUIManager::Notify(UIINotify* notify)
{
	// 通知を入れる
	notifyList_.push_back(notify);
}


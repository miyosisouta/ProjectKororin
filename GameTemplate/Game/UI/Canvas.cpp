#include "stdafx.h"
#include "Scene/GameScene.h"
#include "Canvas.h"
#include "Core/GameUIManager.h"
#include "Core/GameTimer.h"
#include "UI/BlackOverRay.h"
#include "UI/ObjectView.h"
#include "UI/SphereSizeText.h"
#include "UI/Timer.h"



Canvas::Canvas()
{

}
Canvas::~Canvas()
{
	if (blackOverRay_) {
		delete blackOverRay_;
		blackOverRay_ = nullptr;
	}

	if (timer_) {
		delete timer_;
		timer_ = nullptr;
	}

	if (sphereSizeText_) {
		delete sphereSizeText_;
		sphereSizeText_ = nullptr;
	}
}

bool Canvas::Start()
{
	// UIのクラス作成
	{
		blackOverRay_ = new BlackOverRay;
		timer_ = new Timer;
		sphereSizeText_ = new SphereSizeText;
	}

	timer_->Start();
	sphereSizeText_->Start();

	GameUIManager::Get().SetSphereSizeText(sphereSizeText_); // UIManagerにsphereSizeTextの情報を設定
	SetLimitTimer(GameTimer::Get().GetGameTime()); // GameTimerクラスからゲームのプレイ時間をもらいTimerクラスに渡す
	return true;
}

void Canvas::Update()
{
	timer_->Update();
	sphereSizeText_->Update();
}

void Canvas::Render(RenderContext& rc)
{
	timer_->Render(rc);
	sphereSizeText_->Render(rc);
}


void Canvas::SetTimer(const float timer)
{
	if (timer_) {
		timer_->SetTimer(timer);
	}
}

void Canvas::SetLimitTimer(const float timer)
{
	if (timer_) {
		timer_->SetGameTimerLimit(timer);
	}
}

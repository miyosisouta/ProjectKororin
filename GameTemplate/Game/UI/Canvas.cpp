#include "stdafx.h"
#include "Game.h"
#include "Canvas.h"
#include "Core/GameUIManager.h"
#include "UI/BlackOverRay.h"
#include "UI/ObjectView.h"
#include "UI/SphereSizeText.h"
#include "UI/Timer.h"



Canvas::Canvas()
{

}
Canvas::~Canvas()
{
}

bool Canvas::Start()
{
    blackOverRay_ = new BlackOverRay;
    objectView_ = new ObjectView;
    sphereSizeText_ = new SphereSizeText;
    timer_ = new Timer;

    // new‚¾‚¯‚¾‚ÆStart‚ÍŒÄ‚Î‚ê‚È‚¢‚Ì‚Å‚±‚±‚ÅŒÄ‚Ô
    timer_->Start(); 
    sphereSizeText_->Start();

    GameUIManager::Get().SetSphereSizeText(sphereSizeText_);
    GameUIManager::Get().SetObjectView(objectView_);
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



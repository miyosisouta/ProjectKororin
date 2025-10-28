#include "stdafx.h"
#include "Game.h"
#include "Canvas.h"
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

    timer_->Start();
    return true;
}

void Canvas::Update()
{
    timer_->Update();
}

void Canvas::Render(RenderContext& rc)
{
    timer_->Render(rc);
}



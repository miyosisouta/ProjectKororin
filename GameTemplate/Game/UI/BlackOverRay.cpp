#include "stdafx.h"
#include "BlackOverRay.h"
#include "ObjectView.h"
#include "SphereSizeText.h"
#include "Timer.h"

namespace
{
    const std::string FAILPATH = "BackGroundCircle.DDS";
    const int PIXEL_SIZE = 128;
    const Vector4 SPRITE_MULCOLOR = Vector4(1.0f, 1.0f, 1.0f, 0.35f);


	
}

void BlackOverRay::RenderDarkOverlay(SpriteRender* spriteRender, Vector3 pos, Vector3 scale)
{
    std::string spritePath = FindFailPath_Sprite(FAILPATH); // ファイルパスの格納
    spriteRender->Init(spritePath.c_str(), PIXEL_SIZE, PIXEL_SIZE); // 画像の初期化・サイズの設定

    spriteRender->SetPSM(pos, scale, SPRITE_MULCOLOR); // 座標・大きさ・乗算カラーの設定
    spriteRender->Update(); // 初期化情報を更新
}


void BlackOverRay::Initialize(SpriteRender* spriteRender, const std::string failPath, const int pixelSize, const Vector3 pos, const Vector3 scale, const Vector4 mulColor)
{
    std::string spritePath = FindFailPath_Sprite(failPath); // ファイルパスの格納
    spriteRender->Init(spritePath.c_str(), pixelSize, pixelSize); // 画像の初期化・サイズの設定

    spriteRender->SetPSM(pos, scale, mulColor); // 座標・大きさ・乗算カラーの設定
    spriteRender->Update(); // 初期化情報を更新
}







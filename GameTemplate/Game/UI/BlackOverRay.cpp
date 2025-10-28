#include "stdafx.h"
#include "BlackOverRay.h"
#include "ObjectView.h"
#include "SphereSizeText.h"
#include "Timer.h"

namespace
{
    const std::string FAILPATH = "BackGroundCircle.DDS";
    const int PIXEL_SIZE = 256;
    const Vector3 SPRITE_SCALE = Vector3::One;
    const Vector4 SPRITE_MULCOLOR = Vector4::White;
}

void BlackOverRay::RenderDarkOverlay(SpriteRender* spriteRender, Vector3 pos)
{
    Initialize(spriteRender, FAILPATH, PIXEL_SIZE, pos, SPRITE_SCALE, SPRITE_MULCOLOR);
}


void BlackOverRay::Initialize(SpriteRender* spriteRender, const std::string failPath, const int pixelSize, const Vector3 pos, const Vector3 scale, const Vector4 mulColor)
{
    std::string spritePath = FindFailPath_Sprite(failPath); // ファイルパスの格納
    spriteRender->Init(spritePath.c_str(), pixelSize, pixelSize); // 画像の初期化・サイズの設定

    spriteRender->SetPSM(pos, scale, mulColor); // 座標・大きさ・乗算カラーの設定
    spriteRender->Update(); // 初期化情報を更新
}







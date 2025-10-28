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
    std::string spritePath = FindFailPath_Sprite(failPath); // �t�@�C���p�X�̊i�[
    spriteRender->Init(spritePath.c_str(), pixelSize, pixelSize); // �摜�̏������E�T�C�Y�̐ݒ�

    spriteRender->SetPSM(pos, scale, mulColor); // ���W�E�傫���E��Z�J���[�̐ݒ�
    spriteRender->Update(); // �����������X�V
}







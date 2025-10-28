#pragma once
#include "UI/BlackOverRay.h"


class Timer : public BlackOverRay
{
public:
	Timer();
	~Timer();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	/**
	 * @brief UI�̐����������Ԃ����Ƃɉ��悤�X�V
	 */
	void UpdateLineValue();


private:
	SpriteRender clockBackRender_; //!< �^�C�}�[���݂₷�����邽�߂̍��̔w�i
	SpriteRender timeGaugeSprite_; //!< �^�C�}�[�̃X�v���C�g
	SpriteRender timeLineSprite_; //!< �^�C�}�[�̐j
	Quaternion rotation_ = Quaternion::Identity;

	float gameTimer_ = 0.0f;
};


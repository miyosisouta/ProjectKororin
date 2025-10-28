#include "stdafx.h"
#include "Timer.h"
#include "UI/BlackOverRay.h"


namespace
{
	const float GAMEPLAY_TIMELIMIT = 120.0f;
	const Vector2 TIMELINESPRITE_PIVOT = Vector2(0.5f, 0.25f); // �^�C�}�[�̐j�̊�_�̍��W
	const Vector3 TIME_BACKRAY = Vector3(750.0f, 400.0f, 0.0f); // ���̔w�i�̍��W

	// �X�v���C�g�ɕK�v�ȏ��
	struct SpriteRenderInfo 
	{
		std::string filePath; // �t�@�C���p�X
		int size; // �s�N�Z���T�C�Y
		Vector3 pos; // ���W
		Vector3 scale; // �摜�T�C�Y�̔{��
		Vector4 color; // ��Z�J���[


		// �X�v���C�g�ɕK�v�ȏ���ݒ�
		SpriteRenderInfo(const std::string path, const int s, const Vector3& p, const Vector3& scal, const Vector4& c)
			: filePath(path)
			, size(s)
			, pos(p)
			, scale(scal)
			, color(c)
		{
		}
	};


	// �ϐ��쐬
	// SPRITE_RENDER_INFO_LIST[0] = SpriteRenderInfoType()
	// SPRITE_RENDER_INFO_LIST[1] = SpriteRenderInfoType()
	SpriteRenderInfo SPRITE_RENDER_INFO_LIST[] =
	{
		SpriteRenderInfo("TimeGauge.DDS", 256, Vector3(750.0f, 400.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
		SpriteRenderInfo("TimeLimitLine.DDS", 128, Vector3(750.0f, 400.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
	};


	// �摜�̎��
	struct SpriteRenderInfoType
	{
		enum Enum
		{
			TimeGauge,
			TimeLimitLine,
		};
	};
}


Timer::Timer()
{
}

Timer::~Timer()
{
}

bool Timer::Start()
{
	/*** �^�C�}�[���݂₷�����邽�߂̍��̔w�i ***/
	RenderDarkOverlay(&clockBackRender_, TIME_BACKRAY); // �t�@�C���p�X�E������
	clockBackRender_.Update(); // �|�W�V�����Ȃǂ̍X�V


	/***  �^�C�}�[�̃X�v���C�g ***/
	const auto& timeGaugeInfo = SPRITE_RENDER_INFO_LIST[SpriteRenderInfoType::TimeGauge];
	Initialize
	(
		&timeGaugeSprite_,
		timeGaugeInfo.filePath,
		timeGaugeInfo.size,
		timeGaugeInfo.pos,
		timeGaugeInfo.scale,
		timeGaugeInfo.color
	);


	/*** �^�C�}�[�̐j ***/
	const auto& timeLimitLine = SPRITE_RENDER_INFO_LIST[SpriteRenderInfoType::TimeLimitLine];
	Initialize
	(
		&timeLineSprite_,
		timeLimitLine.filePath,
		timeLimitLine.size,
		timeLimitLine.pos, 
		timeLimitLine.scale,
		timeLimitLine.color
	);

	timeLineSprite_.SetPivot(TIMELINESPRITE_PIVOT);
	timeLineSprite_.Update();

	gameTimer_ = GAMEPLAY_TIMELIMIT;
	return true;
}

void Timer::Update()
{
	UpdateLineValue();
	timeLineSprite_.Update();
}

void Timer::Render(RenderContext& rc)
{
	clockBackRender_.Draw(rc);
	timeGaugeSprite_.Draw(rc);
	timeLineSprite_.Draw(rc);
}


void Timer::UpdateLineValue()
{
	gameTimer_ -= g_gameTime->GetFrameDeltaTime();
	float progressRate =gameTimer_ / GAMEPLAY_TIMELIMIT; // ��]�䗦���v�Z
	float angle = progressRate * 360.0f; // ��]�p�x
	rotation_.SetRotationDegZ(angle);
	timeLineSprite_.SetRotation(rotation_); // ��
}

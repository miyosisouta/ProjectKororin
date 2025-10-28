#pragma once
/**
 * @brief UI�V�X�e���S�̂̓����ƊO���Ƃ̐ړ_���Ǘ�
 */

class BlackOverRay;
class ObjectView;
class SphereSizeText;
class Timer;
class Canvas : public IGameObject
{
public:
	Canvas();
	~Canvas();

	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

private:
	BlackOverRay* blackOverRay_ = nullptr;
	ObjectView* objectView_ = nullptr;
	SphereSizeText* sphereSizeText_ = nullptr;
	Timer* timer_ = nullptr;
};


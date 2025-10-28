#pragma once
/**
 * @brief UIシステム全体の統括と外部との接点を管理
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


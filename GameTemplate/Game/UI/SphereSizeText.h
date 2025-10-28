#pragma once
#include "UI/BlackOverRay.h"


class SphereSizeText : public BlackOverRay
{
public:
	~SphereSizeText();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

};


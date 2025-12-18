#pragma once
#include "UI/BlackOverRay.h"

class ObjectView : public BlackOverRay
{
public:
	~ObjectView();

	bool Start() override;
	void Render(RenderContext& rc) override;

public:
	
private:
	SpriteRender AttachObjectBackSprite;
	SpriteRender PlaceHolderSprite;
};


#pragma once
#include "Mario.h"

class CRedRaccoonMario : public CMario
{

public:
	CRedRaccoonMario(float x = 0.0f, float y = 0.0f);
	void Render(Vector2 finalPos);
	void CollidedLeftRight(vector<LPCOLLISIONEVENT>*) override;
	void GetBoundingBox(float&, float&, float&, float&) override;

	void ProcessKeyboard(SKeyboardEvent kEvent);
};


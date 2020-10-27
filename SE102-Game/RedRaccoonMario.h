#pragma once
#include "Mario.h"

class CRedRaccoonMario : public CMario
{

public:
	CRedRaccoonMario(float x = 0.0f, float y = 0.0f);
	void Render(Vector2 finalPos);
	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};


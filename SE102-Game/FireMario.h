#pragma once
#include "Mario.h"

class CFireMario : public CMario
{

public:
	CFireMario(float x = 0.0f, float y = 0.0f);
	void Render(Vector2 finalPos);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};


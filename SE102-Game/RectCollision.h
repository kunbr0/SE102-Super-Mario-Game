#pragma once
#include "GameObject.h"

class RectCollision : public CGameObject
{
public:
	RectCollision(float x, float y);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	
};


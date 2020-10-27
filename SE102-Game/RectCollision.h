#pragma once
#include "GameObject.h"

class CRectCollision : public CGameObject
{
private:
	int width, height;
public:
	CRectCollision(float x, float y, float width, float height);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render(Vector2 finalPos);
	
};


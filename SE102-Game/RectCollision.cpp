#include "RectCollision.h"

CRectCollision::CRectCollision(float x, float y, float width, float height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void CRectCollision::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + this->width;
	bottom = y + this->height;

}

void CRectCollision::Render(Vector2 finalPos) {
	//RenderBoundingBox(finalPos);
	
}


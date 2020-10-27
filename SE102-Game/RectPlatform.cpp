#include "RectPlatform.h"


CRectPlatform::CRectPlatform(float x, float y, float width, float height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void CRectPlatform::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + this->width;
	bottom = y + this->height;

}

void CRectPlatform::Render(Vector2 finalPos) {
	//RenderBoundingBox(finalPos);

}


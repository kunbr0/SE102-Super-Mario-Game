#include "RectCollision.h"

RectCollision::RectCollision(float x, float y) {
	this->x = x;
	this->y = y;
}

void RectCollision::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 16;

}

void RectCollision::Render() {
	RenderBoundingBox();
}


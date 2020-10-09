#include "RectCollision.h"
#include "Game.h"

RectCollision::RectCollision(float x, float y, float width, float height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void RectCollision::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + this->width;
	bottom = y + this->height;

}

void RectCollision::Render() {
	//RenderBoundingBox();
	
}


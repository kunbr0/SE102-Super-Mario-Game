#include "Brick.h"

void CBrick::Render(Vector2 finalPos)
{
	animation_set->at(0)->Render(finalPos, 255);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}
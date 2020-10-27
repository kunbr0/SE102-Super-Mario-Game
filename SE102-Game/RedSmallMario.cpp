#include "RedSmallMario.h"

CRedSmallMario::CRedSmallMario(float x, float y) : CMario(x, y) {
	type = MarioType::RED_SMALL;

}

void CRedSmallMario::Render(Vector2 finalPos)
{
	CMario::Render(finalPos);

}

void CRedSmallMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = x + MARIO_SMALL_BBOX_WIDTH;
	bottom = y + MARIO_SMALL_BBOX_HEIGHT;
}
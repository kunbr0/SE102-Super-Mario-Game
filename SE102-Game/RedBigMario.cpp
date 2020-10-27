#include "RedBigMario.h"

CRedBigMario::CRedBigMario(float x, float y) : CMario(x, y) {
	type = MarioType::RED_BIG;

}

void CRedBigMario::Render(Vector2 finalPos)
{
	CMario::Render(finalPos);
}

void CRedBigMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = x + MARIO_BIG_BBOX_WIDTH;
	bottom = y + MARIO_BIG_BBOX_HEIGHT;
}
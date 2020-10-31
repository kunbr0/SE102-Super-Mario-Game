#include "RedSmallMario.h"

CRedSmallMario::CRedSmallMario(float x, float y) : CMario(x, y) {
	type = MarioType::RED_SMALL;

}

void CRedSmallMario::Render(Vector2 finalPos)
{
	CMario::Render(finalPos);

}


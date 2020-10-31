#include "RedBigMario.h"

CRedBigMario::CRedBigMario(float x, float y) : CMario(x, y) {
	type = MarioType::RED_BIG;

}

void CRedBigMario::Render(Vector2 finalPos)
{
	CMario::Render(finalPos);
}

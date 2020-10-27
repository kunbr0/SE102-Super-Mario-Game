#include "RedRaccoonMario.h"

CRedRaccoonMario::CRedRaccoonMario(float x, float y) : CMario(x, y) {
	type = MarioType::RED_RACCON;

}

void CRedRaccoonMario::Render(Vector2 finalPos)
{
	CMario::Render(finalPos);

}


void CRedRaccoonMario::SetState(int state)
{
	CMario::SetState(state);

	switch (state)
	{
		case MARIO_STATE_PRESS_A:
			if (type == MarioType::RED_RACCON) {
				isShowingSpecialAni = RACCOON_MARIO_ANI_BIG_ATTACK;
				isBoostedSpeed = false;
			}
	}

}


void CRedRaccoonMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = x + RACCOON_MARIO_BIG_BBOX_WIDTH;
	bottom = y + RACCOON_MARIO_BIG_BBOX_HEIGHT;
}

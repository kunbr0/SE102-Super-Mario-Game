#include "RedMario.h"

CRedMario::CRedMario(float x, float y) : CMario(x, y) {

}

void CRedMario::Render(Vector2 finalPos)
{
	string ani;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else {
		if (level == MARIO_LEVEL_RACCOON)
		{
			if (vx == 0) {
				ani = RACCOON_MARIO_ANI_BIG_IDLE;
			}
			else
				ani = RACCOON_MARIO_ANI_BIG_WALK;
			if (isFlying) {
				ani = RACCOON_MARIO_ANI_BIG_FLY;
			}
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			if (vx == 0)
				ani = MARIO_ANI_BIG_IDLE;
			else
				ani = MARIO_ANI_BIG_WALK;
			if (status == STATUS_IS_JUMPING)
				ani = MARIO_ANI_BIG_JUMP;
			if (status == STATUS_IS_FALLING)
				ani = MARIO_ANI_BIG_FALL;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (vx == 0)
				ani = MARIO_ANI_SMALL_IDLE;
			else
				ani = MARIO_ANI_SMALL_WALK;
		}
	}

		
	SHORT exceptionalNX = 1;

	if (isShowingSpecialAni != "-1") {
		ani = isShowingSpecialAni;
		if (ani == MARIO_ANI_BIG_SKID || ani == MARIO_ANI_SMALL_SKID || ani == RACCOON_MARIO_ANI_BIG_SKID)
			exceptionalNX = -1;
	}

	int alpha = 255;
	if (untouchable) alpha = 128;


	bool isFinishAni = CAnimations::GetInstance()->Get(ani)->Render(finalPos, 255, !(nx * exceptionalNX > 0));
	if (isFinishAni) isShowingSpecialAni = "-1";
	
	
	//RenderBoundingBox(finalPos);

}

void CRedMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	if (level == MARIO_LEVEL_RACCOON)
	{
		right = x + RACCOON_MARIO_BIG_BBOX_WIDTH;
		bottom = y + RACCOON_MARIO_BIG_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}
#include "FireMario.h"

CFireMario::CFireMario(float x, float y) : CMario(x, y) {
	this->x = x;
	this->y = y;
}

void CFireMario::Render(Vector2 finalPos)
{
	string ani;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else {
		if (vx == 0)
			ani = FIRE_MARIO_ANI_IDLE;
		else
			ani = FIRE_MARIO_ANI_WALK;
		if (status == STATUS_IS_JUMPING)
			ani = FIRE_MARIO_ANI_JUMP;
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

void CFireMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = x + FIRE_MARIO_BBOX_WIDTH;
	bottom = y + FIRE_MARIO_BBOX_HEIGHT;
}
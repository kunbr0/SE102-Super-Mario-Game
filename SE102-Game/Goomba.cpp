#include "Goomba.h"
CGoomba::CGoomba(float x, float y)
{	
	this->x = x;
	this->y = y;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	/*if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else*/
	bottom = y + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	applyGravity();
	cleanAfterCalcCollision(calcCollision(coObjects));
}

void CGoomba::Render(Vector2 finalPos)
{
	int ani = GOOMBA_ANI_WALKING;
	/*if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}*/

	LPANIMATION a = CAnimations::GetInstance()->Get("ani-goomba-walk");
	a->Render(finalPos, 255);

	//RenderBoundingBox(finalPos);
}

void CGoomba::SetState(int state)
{
	/*CGameObject::SetState(state);*/
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
	}
}

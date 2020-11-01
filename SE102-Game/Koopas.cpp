#include "Koopas.h"
CKoopas::CKoopas(float x, float y)
{
	this->x = x;
	this->y = y;
	SetState(GOOMBA_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;
	bottom = y + GOOMBA_BBOX_HEIGHT;
}

void CKoopas::Kill() {
	this->state = GOOMBA_STATE_CROUCHING;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	applyGravity();
	CGameObject::Update(dt, coObjects);
	UpdateWithCollision(coObjects);
	
}

void CKoopas::Render(Vector2 finalPos)
{
	renderAnimation.AnimationID = state == GOOMBA_STATE_WALKING ? KOOPAS_ANI_WALKING : KOOPAS_ANI_CROUCH;

	LPANIMATION a = CAnimations::GetInstance()->Get(renderAnimation.AnimationID);
	a->Render(finalPos, 255, (nx == 1 ? false : true));

	RenderBoundingBox(finalPos);
}

void CKoopas::SetState(int state)
{
	this->state = state;
	/*CGameObject::SetState(state);*/
	switch (state)
	{
	case GOOMBA_STATE_CROUCHING:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
	}
}

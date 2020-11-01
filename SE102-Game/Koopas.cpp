#include "Koopas.h"

CKoopas::CKoopas(float x, float y)
{
	this->x = x;
	this->y = y;
	this->nx = -1;
	SetState(GOOMBA_STATE_WALKING);
}



void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (state == GOOMBA_STATE_WALKING) {
		right = x + GOOMBA_BBOX_WIDTH;
		bottom = y + GOOMBA_BBOX_HEIGHT;
	}
	else {
		right = x + GOOMBA_BBOX_CROUCH_WIDTH;
		bottom = y + GOOMBA_BBOX_CROUCH_HEIGHT;
	}
		
}

void CKoopas::Kill() {
	
	SetState(GOOMBA_STATE_CROUCHING);
}

void CKoopas::ChangeDirection() {
	vx *= -1;
	dx *= -1;
	nx *= -1;
	vx = nx*GOOMBA_WALKING_SPEED;
}

void CKoopas::CollideLeft(vector<LPCOLLISIONEVENT> coEvents){
	ChangeDirection();
}
void CKoopas::CollideRight(vector<LPCOLLISIONEVENT> coEvents) {
	ChangeDirection();
}

void CKoopas::CollideTop(vector<LPCOLLISIONEVENT> coEvents) {
	if (standingObject == NULL) {
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			LPCOLLISIONEVENT e = coEvents[i];
			if (e->ny < 0) { 
				float left, top, right, bottom;
				e->obj->GetBoundingBox(left, top, right, bottom);
				standingObject = e->obj;
				stadingScope = Vector2(left + MARGIN_STANDING_AREA, right - MARGIN_STANDING_AREA);
				return;
			}

		}
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	applyGravity();
	CGameObject::Update(dt, coObjects);
	if (x + dx < stadingScope.x || x + dx > stadingScope.y) {
		ChangeDirection();
	} 
	UpdateWithCollision(coObjects);
	
}

void CKoopas::Render(Vector2 finalPos)
{
	renderAnimation.AnimationID = state == GOOMBA_STATE_WALKING ? KOOPAS_ANI_WALKING : KOOPAS_ANI_CROUCH;

	LPANIMATION a = CAnimations::GetInstance()->Get(renderAnimation.AnimationID);
	a->Render(finalPos, 255, (nx == -1 ? false : true));

	//RenderBoundingBox(finalPos);
}

void CKoopas::SetState(int state)
{
	this->state = state;
	/*CGameObject::SetState(state);*/
	switch (state)
	{
	case GOOMBA_STATE_CROUCHING:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_CROUCH_HEIGHT - 1;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = -nx*GOOMBA_WALKING_SPEED;
	}
}

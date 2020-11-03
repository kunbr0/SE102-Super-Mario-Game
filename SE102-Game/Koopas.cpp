#include "Koopas.h"

CKoopas::CKoopas(float x, float y)
{
	this->x = x;
	this->y = y;
	this->nx = -1;
	SetState(EEnemyState::LIVE);
}


std::string CKoopas::GetRenderAnimationId(EEnemyState type) {
	switch (type)
	{
	case EEnemyState::LIVE:
		return KOOPAS_ANI_WALKING;
	case EEnemyState::WILL_DIE:
		return KOOPAS_ANI_CROUCH;
	case EEnemyState::DIE:
		return KOOPAS_ANI_CROUCH;
	default:
		break;
	}
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (state.type == EEnemyState::LIVE) {
		right = x + GOOMBA_BBOX_WIDTH;
		bottom = y + GOOMBA_BBOX_HEIGHT;
	} 
	else {
		right = x + GOOMBA_BBOX_CROUCH_WIDTH;
		bottom = y + GOOMBA_BBOX_CROUCH_HEIGHT;
	}

}
void CKoopas::BeingCollided(ETag eTag) {
	SetState(EEnemyState::WILL_DIE);
}

void CKoopas::BeingCollidedLeft(ETag eTag) {
	vx = 0.74;
}

void CKoopas::BeingCollidedRight(ETag eTag) {
	vx = -0.74;
}


void CKoopas::BeingCollidedTop(ETag eTag) {
	SetState(EEnemyState::WILL_DIE);
}


void CKoopas::ChangeDirection() {
	vx *= -1;
	dx *= -1;
	nx *= -1;
	vx = nx*GOOMBA_WALKING_SPEED;
}

void CKoopas::CollidedLeft(vector<LPCOLLISIONEVENT> coEvents){
	ChangeDirection();
}
void CKoopas::CollidedRight(vector<LPCOLLISIONEVENT> coEvents) {
	ChangeDirection();
}

void CKoopas::CollidedTop(vector<LPCOLLISIONEVENT> coEvents) {
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
	CEnemy::Render(finalPos);
	RenderBoundingBox(finalPos);
}

void CKoopas::SetState(EEnemyState newState)
{
	state.type = newState;
	
	switch (newState)
	{
	case EEnemyState::WILL_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_CROUCH_HEIGHT - 1;
		vx = 0;
		vy = 0;
		break;
	case EEnemyState::LIVE:
		vx = -nx*GOOMBA_WALKING_SPEED;
	}
}

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


void CKoopas::BeingCollidedLeftRight(ETag eTag, Vector2 collidePos) {
	if (state.type != EEnemyState::LIVE)
		Kick(collidePos);		
}

void CKoopas::BeingCollidedTop(ETag eTag, Vector2 collidePos) {
	if (state.type == EEnemyState::LIVE)
		ChangeState(EEnemyState::WILL_DIE);
	else
		Kick(collidePos);
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


void CKoopas::Kick(Vector2 pos) {
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	if (pos.x < (left + right) / 2)
		vx = 0.74;
	else
		vx = -0.74;
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

void CKoopas::ChangeState(EEnemyState newState)
{
	switch (newState)
	{
	case EEnemyState::DIE:
		if (state.type == EEnemyState::WILL_DIE)
			SetState(newState);
		break;
	case EEnemyState::WILL_DIE:
		if (state.type == EEnemyState::LIVE)
			SetState(newState);
		break;
	case EEnemyState::LIVE:
			SetState(newState);
		break;
	default:
		break;
	}
}

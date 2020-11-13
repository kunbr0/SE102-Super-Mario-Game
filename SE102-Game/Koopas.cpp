#include "Koopas.h"

CKoopas::CKoopas(float x, float y)
{
	this->x = x;
	this->y = y;
	this->nx = -1;
	
	ChangeState(EEnemyState::LIVE);
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
		return KOOPAS_ANI_WALKING;
	}
}

Vector2 CKoopas::GetBoundingBoxSize() {
	switch (state.type)
	{
	case EEnemyState::LIVE:
		return Vector2(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
	case EEnemyState::WILL_DIE:
		return Vector2(GOOMBA_BBOX_CROUCH_WIDTH, GOOMBA_BBOX_CROUCH_WIDTH);
	case EEnemyState::DIE:
	case EEnemyState::ONESHOTDIE:
		return Vector2(0, 0);
	default:
		return Vector2(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
	}
}




void CKoopas::BeingCollidedLeftRight(EActionTag eActionTag, Vector2 collidePos) {
	if (state.type != EEnemyState::LIVE)
		Kick(collidePos);	
	switch (eActionTag)
	{
	case EActionTag::MARIO_DEFAULT:
		break;
	case EActionTag::MARIO_ATTACK:
		ChangeState(EEnemyState::ONESHOTDIE);
		break;
	default:
		break;
	}
}

void CKoopas::BeingCollidedTop(EActionTag eActionTag, Vector2 collidePos) {
	if (state.type == EEnemyState::LIVE)
		ChangeState(EEnemyState::WILL_DIE);
	else
		Kick(collidePos);
}




void CKoopas::CollidedLeftRight(vector<LPCOLLISIONEVENT>* coEvents){
	ChangeDirection();
}

void CKoopas::CollidedTop(vector<LPCOLLISIONEVENT>* coEvents) {
	InitWtandingScope(coEvents);
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
	ApplyGravity();
	CEnemy::Update(dt, coObjects);
	ChangeDirectionAfterAxisCollide();
	UpdateWithCollision(coObjects);
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
		if (state.type == EEnemyState::LIVE) {
			y -= GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_CROUCH_HEIGHT;
			walkingSpeed = 0;
			SetState(newState);
		}
		break;
	case EEnemyState::LIVE:
		walkingSpeed = GOOMBA_WALKING_SPEED;
		SetState(newState);
		break;
	case EEnemyState::ONESHOTDIE:
			vy = -0.7f;
			SetState(newState);
		break;
	default:
		break;
	}
}

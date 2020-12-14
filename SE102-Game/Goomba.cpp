#include "Goomba.h"

CGoomba::CGoomba(float x, float y)
{
	this->x = x;
	this->y = y;
	this->nx = -1;

	ChangeState(EEnemyState::LIVE);
}


std::string CGoomba::GetAnimationIdFromState() {
	switch (state.type)
	{
	case EEnemyState::LIVE:
	case EEnemyState::ONESHOTDIE:
		return GOOMBA_ANI_WALKING;
	case EEnemyState::WILL_DIE:
		return GOOMBA_ANI_WILL_DIE;
	default:
		return GOOMBA_ANI_WALKING;
	}
}

Vector2 CGoomba::GetBoundingBoxSize(EEnemyState st) {
	switch (st)
	{
	case EEnemyState::LIVE:
		return Vector2(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
	case EEnemyState::WILL_DIE:
		return Vector2(0, 0);
	case EEnemyState::DIE:
	case EEnemyState::ONESHOTDIE:
		return Vector2(0, 0);
	default:
		return Vector2(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
	}
}






void CGoomba::BeingCollidedTopBottom(LPGAMEOBJECT obj) {
	BeingCollided(obj);
	
	if (dynamic_cast<CMario*>(obj)) {
		if (state.type == EEnemyState::LIVE)
			ChangeState(EEnemyState::WILL_DIE, 1000);
		else
			BeingKicked(obj->GetPosition());
	}

}



void CGoomba::CollidedTop(vector<LPCOLLISIONEVENT>* coEvents) {
	InitWtandingScope(coEvents);
}


void CGoomba::ChangeState(EEnemyState newState, DWORD newTimeState) {
	CEnemy::ChangeState(newState, newTimeState);
	if (newState == EEnemyState::WILL_DIE)	y += GOOMBA_BBOX_WILL_DIE_HEIGHT / 2;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state.type == EEnemyState::WILL_DIE) {
		ChangeState(EEnemyState::DIE);
		return;
	}
	if (state.type == EEnemyState::DIE) return;
	ApplyGravity();
	CEnemy::Update(dt, coObjects);
	
	ChangeDirectionAfterAxisCollide();
	UpdateWithCollision(coObjects);

	
}




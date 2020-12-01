#include "Koopas.h"

CKoopas::CKoopas(float x, float y)
{
	this->x = x;
	this->y = y;
	this->nx = -1;
	
	ChangeState(EEnemyState::LIVE);
}


std::string CKoopas::GetAnimationIdFromState() {
	switch (state.type)
	{
	case EEnemyState::LIVE:
		return KOOPAS_ANI_WALKING;
	case EEnemyState::WILL_DIE:
	case EEnemyState::BEING_KICKED:
		return KOOPAS_ANI_CROUCH;
	default:
		return KOOPAS_ANI_WALKING;
	}
}

Vector2 CKoopas::GetBoundingBoxSize(EEnemyState st) {
	switch (st)
	{
	case EEnemyState::LIVE:
		return Vector2(KOOPAS_BBOX_WIDTH, KOOPAS_BBOX_HEIGHT);
	case EEnemyState::WILL_DIE:
	case EEnemyState::BEING_KICKED:
		return Vector2(KOOPAS_BBOX_CROUCH_WIDTH, KOOPAS_BBOX_CROUCH_WIDTH);
	case EEnemyState::DIE:
	case EEnemyState::ONESHOTDIE:
		return Vector2(0, 0);
	default:
		return Vector2(KOOPAS_BBOX_WIDTH, KOOPAS_BBOX_HEIGHT);
	}
}






void CKoopas::BeingCollidedTopBottom(LPGAMEOBJECT obj) {
	BeingCollided(obj);
	
	if (dynamic_cast<CMario*>(obj)) {
		if (state.type == EEnemyState::LIVE)
			ChangeState(EEnemyState::WILL_DIE, 5000);
		else
			BeingKicked(obj->GetPosition());
	}
	
}





void CKoopas::CollidedTop(vector<LPCOLLISIONEVENT>* coEvents) {
	InitWtandingScope(coEvents);
}



void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state.type == EEnemyState::WILL_DIE) ChangeState(EEnemyState::LIVE);
	ApplyGravity();
	CEnemy::Update(dt, coObjects);
	ChangeDirectionAfterAxisCollide();
	UpdateWithCollision(coObjects);
}




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
	case EEnemyState::BEING_KICKED:
		return KOOPAS_ANI_CROUCH;
	case EEnemyState::DIE:
		return KOOPAS_ANI_CROUCH;
	default:
		return KOOPAS_ANI_WALKING;
	}
}

Vector2 CKoopas::GetBoundingBoxSize(EEnemyState st) {
	switch (st)
	{
	case EEnemyState::LIVE:
		return Vector2(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
	case EEnemyState::WILL_DIE:
	case EEnemyState::BEING_KICKED:
		return Vector2(GOOMBA_BBOX_CROUCH_WIDTH, GOOMBA_BBOX_CROUCH_WIDTH);
	case EEnemyState::DIE:
	case EEnemyState::ONESHOTDIE:
		return Vector2(0, 0);
	default:
		return Vector2(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
	}
}

void CKoopas::BeingCollided(LPGAMEOBJECT obj) {
	if (dynamic_cast<CMario*>(obj)) {
		MarioAction objAction = ((CMario*)(obj))->GetAction();
		if (objAction == MarioAction::ATTACK) {
			ChangeState(EEnemyState::ONESHOTDIE);
		}
	}
	else if (dynamic_cast<CFireBullet*>(obj)) {
		ChangeState(EEnemyState::ONESHOTDIE);
	}
}


void CKoopas::BeingCollidedLeftRight(LPGAMEOBJECT obj) {
	BeingCollided(obj);
	if (dynamic_cast<CMario*>(obj)) {
		switch (state.type)
		{
		case EEnemyState::LIVE:
		case EEnemyState::BEING_KICKED:
			KillMario((CMario*)obj);
			break;
		case EEnemyState::WILL_DIE:
			BeingKicked(obj->GetPosition());
			break;
		case EEnemyState::DIE:
			break;
		}
	}
	
}

void CKoopas::BeingCollidedTop(LPGAMEOBJECT obj) {
	BeingCollided(obj);
	CEnemy::BeingCollidedTop(obj);
	if (dynamic_cast<CMario*>(obj)) {
		if (state.type == EEnemyState::LIVE)
			ChangeState(EEnemyState::WILL_DIE);
		else
			BeingKicked(obj->GetPosition());
	}
	
}




void CKoopas::CollidedLeftRight(vector<LPCOLLISIONEVENT>* coEvents){
	ChangeDirection();
}

void CKoopas::CollidedTop(vector<LPCOLLISIONEVENT>* coEvents) {
	InitWtandingScope(coEvents);
}




void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ApplyGravity();
	CEnemy::Update(dt, coObjects);
	DebugOut(ToWSTR(std::to_string(vx) + "\n").c_str());
	ChangeDirectionAfterAxisCollide();
	UpdateWithCollision(coObjects);
}




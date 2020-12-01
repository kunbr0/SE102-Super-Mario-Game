#include "KoopasFly.h"

CKoopasFly::CKoopasFly(float x, float y)
{
	this->x = x;
	this->y = y;
	this->nx = -1;

	ChangeState(EEnemyState::LIVE);
}


std::string CKoopasFly::GetAnimationIdFromState() {
	switch (state.type)
	{
	case EEnemyState::LIVE:
		return KOOPASFLY_ANI_WALKING;
	case EEnemyState::WILL_DIE:
	case EEnemyState::BEING_KICKED:
		return KOOPASFLY_ANI_CROUCH;
	default:
		return KOOPASFLY_ANI_WALKING;
	}
}

Vector2 CKoopasFly::GetBoundingBoxSize(EEnemyState st) {
	switch (st)
	{
	case EEnemyState::LIVE:
		return Vector2(KOOPASFLY_BBOX_WIDTH, KOOPASFLY_BBOX_HEIGHT);
	case EEnemyState::WILL_DIE:
	case EEnemyState::BEING_KICKED:
		return Vector2(KOOPASFLY_BBOX_CROUCH_WIDTH, KOOPASFLY_BBOX_CROUCH_WIDTH);
	case EEnemyState::DIE:
	case EEnemyState::ONESHOTDIE:
		return Vector2(0, 0);
	default:
		return Vector2(KOOPASFLY_BBOX_WIDTH, KOOPASFLY_BBOX_HEIGHT);
	}
}




void CKoopasFly::CollidedTop(vector<LPCOLLISIONEVENT>*) {
	vy = -0.7f;
}

void CKoopasFly::BeingCollidedTopBottom(LPGAMEOBJECT obj) {
	BeingCollided(obj);

	if (dynamic_cast<CMario*>(obj)) {
		if (state.type == EEnemyState::LIVE)
			ChangeState(EEnemyState::WILL_DIE);
		else
			BeingKicked(obj->GetPosition());
	}

}








void CKoopasFly::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += 0.0014f * dt;
	CEnemy::Update(dt, coObjects);
	
	
	UpdateWithCollision(coObjects);
}




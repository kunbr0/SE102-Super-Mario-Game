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
		return GOOMBA_ANI_WALKING;
	case EEnemyState::WILL_DIE:
		return GOOMBA_ANI_WALKING;
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
		return Vector2(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_WIDTH);
	case EEnemyState::DIE:
	case EEnemyState::ONESHOTDIE:
		return Vector2(0, 0);
	default:
		return Vector2(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_WIDTH);
	}
}

void CGoomba::BeingCollided(LPGAMEOBJECT obj) {
	if (dynamic_cast<CMario*>(obj)) {
		MarioAction objAction = ((CMario*)(obj))->GetAction();
		if (objAction == MarioAction::ATTACK) {
			vy = -0.9f;
			SwitchEffect(EExtraEffect::BEING_DAMAGED);
			ChangeState(EEnemyState::WILL_DIE);
		}
	}
	else if (dynamic_cast<CFireBullet*>(obj)) {
		ChangeState(EEnemyState::ONESHOTDIE);
	}
}


void CGoomba::BeingCollidedLeftRight(LPGAMEOBJECT obj) {
	BeingCollided(obj);
	if (dynamic_cast<CMario*>(obj)) {
		if (((CMario*)(obj))->GetAction() == MarioAction::ATTACK) return;
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

void CGoomba::BeingCollidedTop(LPGAMEOBJECT obj) {
	this->BeingCollidedTopBottom(obj);
}

void CGoomba::BeingCollidedTopBottom(LPGAMEOBJECT obj) {
	BeingCollided(obj);
	CEnemy::BeingCollidedTop(obj);
	if (dynamic_cast<CMario*>(obj)) {
		if (state.type == EEnemyState::LIVE)
			ChangeState(EEnemyState::WILL_DIE);
		else
			BeingKicked(obj->GetPosition());
	}

}




void CGoomba::CollidedLeftRight(vector<LPCOLLISIONEVENT>* coEvents) {
	ChangeDirection();
}

void CGoomba::CollidedTop(vector<LPCOLLISIONEVENT>* coEvents) {
	InitWtandingScope(coEvents);
}




void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ApplyGravity();
	CEnemy::Update(dt, coObjects);
	//DebugOut(ToWSTR(std::to_string(vx) + "\n").c_str());
	ChangeDirectionAfterAxisCollide();
	UpdateWithCollision(coObjects);
}




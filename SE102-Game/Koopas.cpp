#include "Koopas.h"

CKoopas::CKoopas(float x, float y)
{
	this->x = x;
	this->y = y;
	this->nx = -1;
	useChangeDirectionAfterAxisCollide = true;
	ChangeState(EEnemyState::LIVE);
}


std::string CKoopas::GetAnimationIdFromState() {
	switch (state.type)
	{
	case EEnemyState::LIVE:
		return KOOPAS_ANI_WALKING;
	case EEnemyState::WILL_DIE:
	case EEnemyState::BEING_HELD:
		return KOOPAS_ANI_CROUCH;
	case EEnemyState::BEING_KICKED:
		return KOOPAS_ANI_BEING_KICKED;
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
	case EEnemyState::BEING_HELD:
		return Vector2(10, 0);
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



void CKoopas::BeingCollided(LPGAMEOBJECT obj) {
	if (dynamic_cast<CMario*>(obj)) {
		MarioAction objAction = ((CMario*)(obj))->GetAction();
		if (objAction == MarioAction::ATTACK) {
			vy = -0.55f;
			
			walkingSpeed = 0;

			SwitchEffect(EExtraEffect::BEING_DAMAGED);
			ChangeState(EEnemyState::WILL_DIE, 5000);
		}
	}
	else if (dynamic_cast<CFireBullet*>(obj)) {
		ChangeState(EEnemyState::ONESHOTDIE);
	}
}


void CKoopas::CollidedTop(vector<LPCOLLISIONEVENT>* coEvents) {
	InitWtandingScope(coEvents);
}



void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state.type == EEnemyState::WILL_DIE || state.type == EEnemyState::BEING_KICKED) ChangeState(EEnemyState::LIVE);
	ApplyGravity();

	DebugOut(ToWSTR(std::to_string((int)state.type) + "\n").c_str());

	CEnemy::Update(dt, coObjects);
	ChangeDirectionAfterAxisCollide();
	UpdateWithCollision(coObjects);
}




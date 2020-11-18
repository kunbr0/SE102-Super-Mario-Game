#include "Enemy.h"
#define ENEMY_ANIMATION_BEING_DAMAGE		"ani-enemy-damaged"


CEnemy::CEnemy() {
	walkingSpeed = 0;
	walkingScope = Vector2(0, 0);
}

void CEnemy::ChangeDirection() {
	dx *= -1;
	nx *= -1;
}

void CEnemy::InitWtandingScope(vector<LPCOLLISIONEVENT>* coEvents) {
	if (walkingScope.x == 0 && walkingScope.y == 0) {
		for (UINT i = 0; i < coEvents->size(); i++)
		{
			LPCOLLISIONEVENT e = coEvents->at(i);
			if (e->ny < 0) {
				float left, top, right, bottom;
				e->obj->GetBoundingBox(left, top, right, bottom);
				
				walkingScope = Vector2(left, right);
				return;
			}
		}
	}
}

void CEnemy::ChangeDirectionAfterAxisCollide() {
	float left, top, right, bottom;
	GetBoundingBox(left, top, right, bottom);
	if ((x + dx < walkingScope.x || x + dx > walkingScope.y - (right-left)) && walkingScope.x != 0 && walkingScope.y != 0) {
		ChangeDirection();
	}
}

void CEnemy::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (GetBoundingBoxSize().x == 0 && GetBoundingBoxSize().y == 0) {
		left = top = right = bottom = 0;
	}
	else {
		left = x;
		top = y;
		right = x + GetBoundingBoxSize().x;
		bottom = y + GetBoundingBoxSize().y;
	}
	
}


void CEnemy::SetState(EEnemyState newState, DWORD timeState) {
	Vector2 currentSize = GetBoundingBoxSize();
	Vector2 newSize = GetBoundingBoxSize(newState);
	state.type = newState;
	state.timeState = timeState;
	state.timeBegin = GetTickCount64();
	if (newSize.x == 0 && newSize.y == 0) return;
	this->y -= (newSize.y - currentSize.y);
	this->x -= (newSize.x - currentSize.x);
}

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vx = walkingSpeed * nx;
	ChangeEffect(EEnemyEffect::NONE);
	CGameObject::Update(dt, coObjects);
}

std::string CEnemy::GetRenderAnimationId(EEnemyEffect effctType) {
	switch (effctType)
	{
	case EEnemyEffect::BEING_DAMAGED:
			return ENEMY_ANIMATION_BEING_DAMAGE;
	default:

		return ENEMY_ANIMATION_BEING_DAMAGE;
	}
}

void CEnemy::Render(Vector2 finalPos) {
	RenderBoundingBox(finalPos);
	CAnimations::GetInstance()->Get(GetRenderAnimationId(state.type))->Render(finalPos,Vector2(-nx, ny) , 255);
	if(effect.type != EEnemyEffect::NONE)
		CAnimations::GetInstance()->Get(GetRenderAnimationId(effect.type))->Render(
			Vector2(finalPos.x +(effect.initPosition.x - this->x), finalPos.y + (effect.initPosition.y - this->y)), 
			Vector2(-nx, ny), 255);
}

void CEnemy::OnHadCollided(LPGAMEOBJECT obj) {
	this->BeingCollided(obj);
};

void CEnemy::BeingCollidedTop(LPGAMEOBJECT obj) {
	if (dynamic_cast<CMario*>(obj)) {
		((CMario*)(obj))->BeingBouncedAfterJumpInTopEnemy();
	}
};


void CEnemy::BeingKicked(Vector2 pos) {
	
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	if (pos.x < (left + right) / 2)
		nx = 1;
	else
		nx = -1;
	walkingSpeed = 0.74;
	ChangeState(EEnemyState::BEING_KICKED);
}

void CEnemy::KillMario(CMario* mario) {
	mario->BeingKilled();
}

void CEnemy::SwitchToDamageEffect() {
	ChangeEffect(EEnemyEffect::BEING_DAMAGED, 500);
	effect.initPosition = Vector2(this->x, this->y);
}

void CEnemy::ChangeEffect(EEnemyEffect newEffect, DWORD timeEffect) {
	if (GetTickCount64() < effect.timeEffect + effect.timeBegin) return;
	effect.timeBegin = GetTickCount64();
	effect.timeEffect = timeEffect;
	effect.type = newEffect;
}
	

void CEnemy::ChangeState(EEnemyState newState)
{
	switch (newState)
	{
	case EEnemyState::DIE:
		if (state.type == EEnemyState::WILL_DIE)
			SetState(newState);
		break;
	case EEnemyState::WILL_DIE:
		if (state.type == EEnemyState::LIVE) {
			walkingSpeed = 0;
			SetState(newState);
		}
		break;
	case EEnemyState::BEING_KICKED:
		if (state.type == EEnemyState::WILL_DIE) {
			SetState(newState);
		}
		break;
	case EEnemyState::LIVE:
		walkingSpeed = GetDefaultWalkingSpeed();
		SetState(newState);
		break;
	case EEnemyState::ONESHOTDIE:
		vy = -0.7f;
		ny = -1;
		SetState(newState);
		break;
	default:
		break;
	}
}

#include "Enemy.h"
#include "PlayScene.h"
#include "AddingPointEffect.h"

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
		left = x - GetBoundingBoxSize().x / 2;
		top = y - GetBoundingBoxSize().y / 2;
		right = x + GetBoundingBoxSize().x / 2;
		bottom = y + GetBoundingBoxSize().y / 2;
	}
	
}


void CEnemy::SetState(EEnemyState newState, DWORD timeState) {
	Vector2 currentSize = GetBoundingBoxSize();
	Vector2 newSize = GetBoundingBoxSize(newState);
	state.type = newState;
	state.timeState = timeState;
	state.timeBegin = GetTickCount64();
	if (newSize.x == 0 && newSize.y == 0) return;
	this->y -= (newSize.y - currentSize.y) / 2;
	this->x -= (newSize.x - currentSize.x) / 2;
}

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vx = walkingSpeed * nx;
	SetEffect(EExtraEffect::NONE);
	CGameObject::Update(dt, coObjects);
}


void CEnemy::Render(Vector2 finalPos) {
	if (state.type == EEnemyState::DIE) return;
	RenderBoundingBox(finalPos);
	CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(finalPos,Vector2(-nx, ny) , 255);
	RenderExtraEffect(finalPos);
	
}


void CEnemy::OnHadCollided(LPGAMEOBJECT obj) {
	this->BeingCollided(obj);
};

void CEnemy::BeingCollidedTop(LPGAMEOBJECT obj) {
	if (dynamic_cast<CMario*>(obj)) {
		((CMario*)(obj))->BeingBouncedAfterJumpInTopEnemy();
	}
	this->BeingCollidedTopBottom(obj);
};

void CEnemy::CollidedLeftRight(vector<LPCOLLISIONEVENT>* coEvents) {
	ChangeDirection();
}

void CEnemy::BeingCollidedLeftRight(LPGAMEOBJECT obj) {
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

void CEnemy::BeingCollided(LPGAMEOBJECT obj) {
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



void CEnemy::KillMario(CMario* mario) {
	mario->BeingKilled();
}



void CEnemy::ChangeState(EEnemyState newState, DWORD newTimeState)
{
	if (GetTickCount64() < state.timeBegin + state.timeState) return;
	switch (newState)
	{
	case EEnemyState::DIE:
		if (state.type == EEnemyState::WILL_DIE)
			SetState(newState, newTimeState);
		break;
	case EEnemyState::WILL_DIE:
		if (state.type == EEnemyState::LIVE) {
			((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEffects(new CAddingPointEffect(GetPosition(), Vector2(0, -0.11)));
			walkingSpeed = 0;
			SetState(newState, newTimeState);
		}
		break;
	case EEnemyState::BEING_KICKED:
		if (state.type == EEnemyState::WILL_DIE) {
			((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEffects(new CAddingPointEffect(GetPosition(), Vector2(0, -0.11)));
			SetState(newState, newTimeState);
		}
		break;
	case EEnemyState::LIVE:
		walkingSpeed = GetDefaultWalkingSpeed();
		SetState(newState, newTimeState);
		break;
	case EEnemyState::ONESHOTDIE:
		vy = -0.7f;
		ny = -1;
		((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEffects(new CAddingPointEffect(GetPosition(), Vector2(0, -0.11)));
		SetState(newState, newTimeState);
		break;
	default:
		break;
	}
}

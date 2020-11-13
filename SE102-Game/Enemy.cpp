#include "Enemy.h"
#include "Mario.h"



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
	state.type = newState;
	state.time = timeState;
}

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vx = walkingSpeed * nx;
	CGameObject::Update(dt, coObjects);
}

void CEnemy::Render(Vector2 finalPos) {
	RenderBoundingBox(finalPos);
	CAnimations::GetInstance()->Get(GetRenderAnimationId(state.type))->Render(finalPos, 255, nx == 1 ? true : false);
}

void CEnemy::OnHasCollided(LPGAMEOBJECT obj) {
	if (dynamic_cast<CMario*>(obj)) {
		if (((CMario*)(obj))->GetAction() == MarioAction::ATTACK) {
			this->BeingCollidedLeftRight(EActionTag::MARIO_ATTACK, ((CMario*)(obj))->GetPosition());
		}
	}
	
};

#include "RaccoonAttackBoundingBox.h"
#include "Enemy.h" 
#include "GoldenBrick.h"

CRaccoonAttackBoundingBox::CRaccoonAttackBoundingBox() {
	this->x = 0;
	this->y = 0;
}

void CRaccoonAttackBoundingBox::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (!isOpening) {
		left = top = right = bottom = 0;
		return;
	}
	left = this->x - size.x / 2;
	top = this->y - size.y / 2;
	right = this->x + size.x / 2;
	bottom = this->y + size.y / 2;
}

//void CRaccoonAttackBoundingBox::CollidedTop(vector<LPCOLLISIONEVENT>* coObjList) {
//	int a = 9;
//}
//
//void CRaccoonAttackBoundingBox::CollidedBottom(vector<LPCOLLISIONEVENT>* coObjList) {
//	int a = 9;
//}
//
//void CRaccoonAttackBoundingBox::CollidedLeftRight(vector<LPCOLLISIONEVENT>* coObjList) {
//	int a = 9;
//}


void CRaccoonAttackBoundingBox::OnHadCollided(LPGAMEOBJECT obj) {
	if (dynamic_cast<CEnemy*>(obj) || dynamic_cast<CGoldenBrick*>(obj)) {
		isOpening = false;
		hasAttacked = true;
	}
}

void CRaccoonAttackBoundingBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vx = vy = 0;
	CGameObject::Update(dt);
	UpdateWithCollision(coObjects);
}


void CRaccoonAttackBoundingBox::Render(Vector2 finalPos) {
	RenderBoundingBox(finalPos);
}

void CRaccoonAttackBoundingBox::UpdatePosition(Vector2 newPos) {
	this->x = newPos.x;
	this->y = newPos.y;
}

void CRaccoonAttackBoundingBox::SetIsOpening(bool a) {
	if (a == false) {
		isOpening = a;
	}
	else {
		if (!hasAttacked) isOpening = a;
	}
}


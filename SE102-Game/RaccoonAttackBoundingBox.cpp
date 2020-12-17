#include "RaccoonAttackBoundingBox.h"
#include "Enemy.h" 
#include "GoldenBrick.h"

CRaccoonAttackBoundingBox::CRaccoonAttackBoundingBox() {

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

void CRaccoonAttackBoundingBox::Collided(vector<LPCOLLISIONEVENT>* coObjList) {
	/*if (hasAttacked) return;
	for (int i = 0; i < coObjList->size(); i++) {
		LPGAMEOBJECT obj = coObjList->at(i)->obj;
		if (dynamic_cast<CEnemy*>(obj) || dynamic_cast<CGoldenBrick*>(obj))
			hasAttacked = true;
	}
	CGameObject::Collided(coObjList);*/
}

void CRaccoonAttackBoundingBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
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
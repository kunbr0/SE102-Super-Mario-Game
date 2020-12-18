#include "Leaf.h"
#include "Mathf.h"
#include "Mario.h"
#define LEFT_SIZE	48
#define LEAF_GRAVITY	0.00015f
#define LEAF_SPEED	    0.003f
#define LEAF_AMPLITUDE	60 

CLeaf::CLeaf(Vector2 initPos) {
	this->x = initPos.x;
	this->y = initPos.y;
	vx = vy = 0;
	initPosition = initPos;
	beginFalling = GetTickCount64();
	isTemp = true;
	vy = -0.2f;
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vy += LEAF_GRAVITY * dt;
	if (vy > vyMax) vy = vyMax;
	CGameObject::Update(dt, coObjects);
	UpdateWithCollision(coObjects);
	float time = GetTickCount64() - beginFalling;
	if(vy > 0)
		x = initPosition.x + LEAF_AMPLITUDE * cos(LEAF_SPEED * time * 1.0f - Mathf::Pi / 2);
}


void CLeaf::Render(Vector2 finalPos) {
	CAnimations::GetInstance()->Get("ani-super-leaf-red")->Render(finalPos, Vector2(-nx, ny), 255);
}



void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - LEFT_SIZE / 2;
	top = y - LEFT_SIZE / 2;
	right = x + LEFT_SIZE / 2;
	bottom = y + LEFT_SIZE / 2;
}

void CLeaf::Collided(vector<LPCOLLISIONEVENT>* coEvents) {
	for (int i = 0; i < coEvents->size(); i++) {
		if (dynamic_cast<CMario*>(coEvents->at(i)->obj)) {
			((CMario*)(coEvents->at(i)->obj))->ChangeAction(MarioAction::UPGRADE_LV);
			isTemp = false;
		}
	}
}
#include "Leaf.h"
#define LEFT_SIZE	48
#define LEAF_GRAVITY	0.003f
#define LEAF_SPEED	    0.003f
#define LEAF_AMPLITUDE	80 

CLeaf::CLeaf(Vector2 initPos) {
	this->x = initPos.x;
	this->y = initPos.y;
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vy = LEAF_GRAVITY * dt;

	CGameObject::Update(dt, coObjects);
	y += dy;
	//x = startPos.x + LEAF_AMPLITUDE * cos(LEAF_SPEED * time - Math::pi / 2);
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
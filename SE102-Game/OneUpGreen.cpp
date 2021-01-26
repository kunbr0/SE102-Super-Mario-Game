#include "OneUpGreen.h"
#define LEFT_SIZE	48
#define LEAF_GRAVITY	0.095f
#define LEAF_SPEED	    0.0045f
#define LEAF_AMPLITUDE	60 

COneUpGreen::COneUpGreen(Vector2 initPos) : CLeaf(initPos) {

}

void COneUpGreen::Render(Vector2 finalPos) {
	CAnimations::GetInstance()->Get("ani-1-up-mushroom")->Render(finalPos, Vector2(-nx, ny), 255);
}

void COneUpGreen::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vy += LEAF_GRAVITY * dt;
	vx = 0.08f;
	if (vy > vyMax) vy = vyMax;
	CGameObject::Update(dt, coObjects);
	UpdateWithCollision(coObjects);
}
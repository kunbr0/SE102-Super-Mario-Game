#include "Coin.h"
#include "AddingPointEffect.h"
#include "PlayScene.h"


CCoin::CCoin(Vector2 initPos) {
	this->x = initPos.x;
	this->y = initPos.y;
	isTemp = true;
}

void CCoin::Collided(vector<LPCOLLISIONEVENT>*) {
	isTemp = false;
	((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEffects(new CAddingPointEffect(Vector2(x, y), Vector2(0, -0.13)));

}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt);
	UpdateWithCollision(coObjects);
}


void CCoin::Render(Vector2 finalPos) {
	CAnimations::GetInstance()->Get("ani-coin")->Render(finalPos, Vector2(-nx, ny), 255);
	
}



void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - COIN_SIZE / 2;
	top = y - COIN_SIZE / 2;
	right = x + COIN_SIZE / 2;
	bottom = y + COIN_SIZE / 2;
}
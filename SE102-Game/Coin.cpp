#include "Coin.h"



CCoin::CCoin(Vector2 initPos) {
	this->x = initPos.x;
	this->y = initPos.y;
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
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
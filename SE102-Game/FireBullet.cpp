#include "FireBullet.h"
#define VELOCITY_X_FIRE_BULLET		0.320f 
#define VELOCITY_Y_FIRE_BULLET		0.040f 
#define DELTA_WIDTH_IN_LEFT_SIDE	80

CFireBullet::CFireBullet(float x, float y, int nx) {
	this->x = x;
	this->y = y;
	this->nx = nx;
}

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	x += VELOCITY_X_FIRE_BULLET * dt * nx ;
	// x = -1 y = 1
	// x = 1 y = 0
	// y = ax+b
	// 1 = -x+b
	// 0 = x+b
	y += VELOCITY_Y_FIRE_BULLET * dt;
}

void CFireBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FIRE_BULLET_WIDTH;
	bottom = y + FIRE_BULLET_HEIGHT;

}



void CFireBullet::Render(Vector2 finalPos) {
	CAnimations::GetInstance()->Get(ANI_FIRE_BULLET)
		->Render(finalPos, 255, !(nx > 0));

}
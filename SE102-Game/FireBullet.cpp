#include "FireBullet.h"
#include "Koopas.h"
#define VELOCITY_X_FIRE_BULLET			0.028125f 
#define VELOCITY_Y_FIRE_BULLET			0.14f 
#define VELOCITY_Y_FIRE_BULLET_BOUNCE	0.485f 
#define INITIAL_DELTA_Y_FIRE_BULLET		10

#define DELTA_WIDTH_IN_LEFT_SIDE	80

CFireBullet::CFireBullet(float x, float y, int nx) {
	this->x = x;
	this->y = y;
	this->nx = nx;
	this->isDisable = true;
}

void CFireBullet::PrepareForShooting() {
	vy = VELOCITY_Y_FIRE_BULLET;
}

void CFireBullet::UpdatePos(Vector2 pos, int nx) {
	this->x = pos.x;
	this->y = pos.y + INITIAL_DELTA_Y_FIRE_BULLET;
	this->nx = nx;
}


void CFireBullet::CollidedLeft(vector<LPCOLLISIONEVENT>* coEvents) {
	this->isDisable = true;
	for (UINT i = 0; i < coEvents->size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents->at(i);
		if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Goomba 
		{
			//((CKoopas*)e->obj)->BeingCollided(tag, Vector2(x,y));
			
		}
	}
}

void CFireBullet::CollidedRight(vector<LPCOLLISIONEVENT>* coEvents) {
	this->isDisable = true;
	for (UINT i = 0; i < coEvents->size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents->at(i);
		if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Goomba 
		{
			//((CKoopas*)e->obj)->BeingCollided(tag, Vector2(x,y));

		}
	}
}

void CFireBullet::CollidedTop(vector<LPCOLLISIONEVENT>* coEvents) {
	vy = -VELOCITY_Y_FIRE_BULLET_BOUNCE;
}

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {


	vx = VELOCITY_X_FIRE_BULLET*nx*dt;
	ApplyGravity();
	CGameObject::Update(dt, coObjects);
	UpdateWithCollision(coObjects);
	
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
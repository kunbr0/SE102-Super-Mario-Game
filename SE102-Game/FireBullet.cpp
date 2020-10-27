#include "FireBullet.h"
#define VELOCITY_X_FIRE_BULLET			0.370f 
#define VELOCITY_Y_FIRE_BULLET			0.14f 
#define VELOCITY_Y_FIRE_BULLET_BOUNCE	0.39f 
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

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	dt = 20;

	vx = VELOCITY_X_FIRE_BULLET*nx;
	

	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	applyGravity();

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	
	CalcPotentialCollisions(coObjects, coEvents);

	
	
	
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;

		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) {
			vx = 0;
			isDisable = true;
		}

		// Co va cham theo chieu Y
		if (ny != 0) {
			vy = 0;
			vy = -VELOCITY_Y_FIRE_BULLET_BOUNCE;
		}



		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			/*if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}*/
		}
	}

	
	
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	
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
#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CMario::CMario(float x, float y) : CGameObject()
{
	
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	isShowingSpecialAni = "-1";
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	dt = 20;


	

	if (abs(vx) < VELOCITY_X_MAX) vx += ax;
	if (vx > 0) {
		vx += -ACCELERATION_FRICTION;
		if (vx < 0) vx = 0;
	}
	if (vx < 0) {
		vx += ACCELERATION_FRICTION;
		if (vx > 0) vx = 0;
	}
	
	// SKID
	
	if (vx * nx < 0 && abs(vx) > VELOCITY_X_MIN_FOR_SKID) {
		vx = -nx * VELOCITY_X_AFTER_SKID;
		if (level == 1) isShowingSpecialAni = MARIO_ANI_SMALL_SKID;
		else if (level == 2) isShowingSpecialAni = MARIO_ANI_BIG_SKID;
		else if (level == 3) isShowingSpecialAni = RACCOON_MARIO_ANI_BIG_SKID;
	}
	
	



	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	applyGravity();

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (dy > 0) {
			status = STATUS_IS_FALLING;
			//if (dy > 13) isFlying = false;
		}
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

		if (nx != 0) vx = 0;

		// Co va cham theo chieu Y
		if (ny != 0) {
			vy = 0;
			isFlying = false;
			if (ny < 0) status = STATUS_IS_IDLING_IN_SOMETHING;
		}



		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	isBoostedSpeed = false;
	ax = 0;
	if (vx != 0) {
		int a = 9;
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CMario::Render(Vector2 finalPos) {};

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {}


void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_PRESS_A:
		if (vx == 0 && level == 3 ) {
			isShowingSpecialAni = RACCOON_MARIO_ANI_BIG_ATTACK;
		}
		else {
			isBoostedSpeed = true;
		}
		break;

	case MARIO_STATE_WALKING_RIGHT:
		//vx = MARIO_WALKING_SPEED * (1 + (isBoostedSpeed ? 1 : 0) * MARIO_WALKING_BOOST_RATE);
		ax = ACCELERATION_X_WALK;
		nx = 1;
		break;

	case MARIO_STATE_WALKING_LEFT:
		//vx = -MARIO_WALKING_SPEED * (1 + (isBoostedSpeed ? 1 : 0) * MARIO_WALKING_BOOST_RATE);
		ax = -ACCELERATION_X_WALK;
		nx = -1;
		break;

	case MARIO_STATE_JUMP_X:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		if (status == STATUS_IS_IDLING_IN_SOMETHING) {
			vy = -MARIO_JUMP_SPEED_Y;
			status = STATUS_IS_JUMPING;
		}
		break;
		
	case MARIO_STATE_JUMP_S:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		if (status == STATUS_IS_IDLING_IN_SOMETHING) {
			vy = -MARIO_JUMP_SPEED_Y * 1.20f;
			status = STATUS_IS_JUMPING;
		}
		break;
	case MARIO_STATE_RACCOON_FLY: 
		vy = -MARIO_FLY_SPEED_Y;
		isFlying = true;
		break;

	case MARIO_STATE_IDLE:
		vx = 0;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;

	case MARIO_STATE_NONE_PRESS_KEY:
		//friction = ACCELERATION_FRICTION; 
		break;
	}

}

void CMario::SetLevel(int lv) {
	if (lv < 1 || lv > 3) return;
	float l1, t1, r1, b1;
	GetBoundingBox(l1, t1, r1, b1);
	level = lv;
	float l2, t2, r2, b2;
	GetBoundingBox(l2, t2, r2, b2);
	y -= b2 - b1;
}


/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}


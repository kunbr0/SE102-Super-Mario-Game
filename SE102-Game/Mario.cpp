#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CMario::CMario(float x, float y) : CGameObject()
{
	type = 1;
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	//SetAnimationSet(CAnimationSets::GetInstance()->Get("mario"));
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	dt = 20;
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	//vy += MARIO_GRAVITY * dt;
<<<<<<< HEAD
	//applyGravity();
=======
	applyGravity();
>>>>>>> 538793a0c457bccbeca438980298e425d334832d

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
<<<<<<< HEAD
		
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		
		if (ny != 0) { 
			vy = 0;
			if (ny < 0) status = STATUS_IS_IDLING_IN_SOMETHING;
		}
		
		
=======

		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;

		if (ny != 0) {
			vy = 0;
			if (ny < 0) status = STATUS_IS_IDLING_IN_SOMETHING;
		}


>>>>>>> 538793a0c457bccbeca438980298e425d334832d

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

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render(Vector2 finalPos)
{
	string ani;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else
		if (type == 1) {
			if (level == MARIO_LEVEL_RACCOON)
			{
				if (vx == 0)
					ani = RACCOON_MARIO_ANI_BIG_IDLE;
				else
					ani = RACCOON_MARIO_ANI_BIG_WALK;


			}
			else if (level == MARIO_LEVEL_BIG)
<<<<<<< HEAD
			{
				if (vx == 0)
					ani = MARIO_ANI_BIG_IDLE;
				else
					ani = MARIO_ANI_BIG_WALK;
				if (status == STATUS_IS_JUMPING)
					ani = MARIO_ANI_BIG_JUMP;
				if (status == STATUS_IS_FALLING)
					ani = MARIO_ANI_BIG_FALL;
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (vx == 0)
					ani = MARIO_ANI_SMALL_IDLE;
				else
					ani = MARIO_ANI_SMALL_WALK;
			}
		}
		else if(type == 2) {
=======
			{
				if (vx == 0)
					ani = MARIO_ANI_BIG_IDLE;
				else
					ani = MARIO_ANI_BIG_WALK;
				if (status == STATUS_IS_JUMPING)
					ani = MARIO_ANI_BIG_JUMP;
				if (status == STATUS_IS_FALLING)
					ani = MARIO_ANI_BIG_FALL;
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (vx == 0)
					ani = MARIO_ANI_SMALL_IDLE;
				else
					ani = MARIO_ANI_SMALL_WALK;
			}
		}
		else if (type == 2) {
>>>>>>> 538793a0c457bccbeca438980298e425d334832d
			if (vx == 0)
				ani = FIRE_MARIO_ANI_IDLE;
			else
				ani = FIRE_MARIO_ANI_WALK;
			if (status == STATUS_IS_JUMPING)
				ani = FIRE_MARIO_ANI_JUMP;
		}
		else if (type == 3) {
			if (vx == 0)
				ani = HAMMER_MARIO_ANI_IDLE;
			else
				ani = HAMMER_MARIO_ANI_WALK;
			if (status == STATUS_IS_JUMPING)
				ani = HAMMER_MARIO_ANI_JUMP;
		}
		else if (type == 4) {
			if (vx == 0)
				ani = FROG_MARIO_ANI_IDLE;
			else
				ani = FROG_MARIO_ANI_WALK;
			if (status == STATUS_IS_JUMPING)
				ani = FROG_MARIO_ANI_JUMP;
		}

	int alpha = 255;
	if (untouchable) alpha = 128;
<<<<<<< HEAD
	
	/*CAnimation* a = CAnimations::GetInstance()->Get(ani);
	if(nx>=0)
		a->Render(x, y, 255);
	else
		a->Render(x, y, 255);*/
	
	CAnimations::GetInstance()->Get(ani)->Render(finalPos, 255);
	RenderBoundingBox(finalPos);
=======

	CAnimation* a = CAnimations::GetInstance()->Get(ani);
	if (nx >= 0)
		a->Render(x, y, 255);
	else
		a->Render(x, y, 255, D3DXVECTOR2(-1.0f, 1.0f));


	RenderBoundingBox();
>>>>>>> 538793a0c457bccbeca438980298e425d334832d
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
<<<<<<< HEAD
	case MARIO_STATE_JUMP_X: 
=======
	case MARIO_STATE_JUMP_X:
>>>>>>> 538793a0c457bccbeca438980298e425d334832d
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		if (status == STATUS_IS_IDLING_IN_SOMETHING) {
			vy = -MARIO_JUMP_SPEED_Y;
			status = STATUS_IS_JUMPING;
<<<<<<< HEAD
		break;
	}
	case MARIO_STATE_JUMP_S:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		if (status == STATUS_IS_IDLING_IN_SOMETHING) {
			vy = -MARIO_JUMP_SPEED_Y*1.20f;
			status = STATUS_IS_JUMPING;
			break;
	}
=======
			break;
		}
	case MARIO_STATE_JUMP_S:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		if (status == STATUS_IS_IDLING_IN_SOMETHING) {
			vy = -MARIO_JUMP_SPEED_Y * 1.20f;
			status = STATUS_IS_JUMPING;
			break;
		}
>>>>>>> 538793a0c457bccbeca438980298e425d334832d
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (type == 1) {
		if (level == MARIO_LEVEL_RACCOON)
		{
			right = x + RACCOON_MARIO_BIG_BBOX_WIDTH;
			bottom = y + RACCOON_MARIO_BIG_BBOX_HEIGHT;
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
		else
		{
			right = x + MARIO_SMALL_BBOX_WIDTH;
			bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		}
	}
	else if (type == 2) {
		right = x + FIRE_MARIO_BBOX_WIDTH;
		bottom = y + FIRE_MARIO_BBOX_HEIGHT;
	}
	else if (type == 3) {
		right = x + HAMMER_MARIO_BBOX_WIDTH;
		bottom = y + HAMMER_MARIO_BBOX_HEIGHT;
	}
	else if (type == 4) {
		if (status == STATUS_IS_IDLING_IN_SOMETHING) {
			right = x + FROG_MARIO_BBOX_WIDTH_LIE;
			bottom = y + FROG_MARIO_BBOX_HEIGHT_LIE;
		}
		else {
			right = x + FROG_MARIO_BBOX_WIDTH;
			bottom = y + FROG_MARIO_BBOX_HEIGHT;
		}
<<<<<<< HEAD
		
=======

>>>>>>> 538793a0c457bccbeca438980298e425d334832d
	}
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


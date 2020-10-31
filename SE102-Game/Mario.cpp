#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CMario::CMario(float x, float y) : CGameObject()
{

	untouchable = 0;

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	dt = 20;

	float vxmax = isBoostedSpeed ? VELOCITY_X_SPEEDUP_MAX : VELOCITY_X_MAX;
	
	// Increase velocity if in limit
	if (abs(vx) < vxmax)
		vx += ax * ( isBoostedSpeed ? ACCELERATION_X_RUN_RATIO : 1);

	applyFriction();
	applyGravity();

	// Calculate dx, dy 
	CGameObject::Update(dt);

	SCollisionResult result = calcCollision(coObjects);
	
	// No collision occured, proceed normally
	if (!result.isCollided && dy > 7) {
		ChangeAction(MarioAction::FALL);
	}
	else {
		if (result.ny < 0 && state.action != MarioAction::CROUCH) {
			ChangeAction(MarioAction::IDLE);
			if (powerX - 100 > 0 && abs(vx) < VELOCITY_X_MIN_FOR_RUN)
				powerX -= 100;
		} 

		// Collision logic with other objects
		for (UINT i = 0; i < result.coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = result.coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					/*if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}*/
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						/*if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}*/
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
	cleanAfterCalcCollision(result);

	if (vx != 0) {
		if(abs(vx) < VELOCITY_X_MIN_FOR_RUN && state.action != MarioAction::CROUCH)
			ChangeAction(MarioAction::WALK);
		else
			if(powerX < 6000)
				ChangeAction(MarioAction::RUN);
			else
				ChangeAction(MarioAction::SPEEDUP);
	}
	// SKID
	if (vx * nx < 0 && abs(vx) > VELOCITY_X_MIN_FOR_SKID) {
		ChangeAction(MarioAction::SKID);
		
	}

	if (powerX + abs(dx) * 75 < 7000 && abs(vx) > VELOCITY_X_MIN_FOR_RUN) {
		powerX += abs(dx)*10;
	}
	else {
		if(powerX-3 > 0)
			powerX -= 3;
	}
	
	DebugOut(ToWSTR(std::to_string(powerX) + "\n").c_str());
	

	ResetTempValues(); // Set all temp values to initial value;
}




void CMario::Render(Vector2 finalPos) {
	
	if (state.action == MarioAction::SKID) {
		SRenderAnimation ani;
		ani.AnimationID = GetAnimationId(state.action);
		ani.isFlipY = true;
		ChangeRenderAnimation(ani);
	}
	else {
		ChangeRenderAnimation(GetAnimationId(state.action));
	}
	

	

	int alpha = 255;
	if (untouchable) alpha = 128;

	//
	RenderBoundingBox(finalPos);
	if (type == MarioType::RED_RACCON && nx == 1) finalPos.x -= 15;
	
	CAnimations::GetInstance()->Get(renderAnimation.AnimationID)->Render(finalPos, 255, renderAnimation.isFlipY ? !(nx == 1 ? false : true) : (nx == 1 ? false : true));
	


	
};

Vector2 CMario::GetBoundingBoxSize(MarioType mType, MarioAction mAction) {
	if(mAction == MarioAction::CROUCH) return Vector2(MARIO_SMALL_BBOX_WIDTH, MARIO_SMALL_BBOX_HEIGHT);
	switch (mType)
	{
	case MarioType::RED_SMALL:
		return Vector2(MARIO_SMALL_BBOX_WIDTH, MARIO_SMALL_BBOX_HEIGHT);
	
	case MarioType::RED_BIG:
	case MarioType::FIRE:
	case MarioType::RED_RACCON:
		return Vector2(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT);
	
	
	
	default:
		return Vector2(MARIO_SMALL_BBOX_WIDTH, MARIO_SMALL_BBOX_HEIGHT);
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = x + GetBoundingBoxSize(type, state.action).x;
	bottom = y + GetBoundingBoxSize(type, state.action).y;
}


void CMario::ProcessKeyboard(SKeyboardEvent kEvent)
{
	switch (kEvent.key)
	{
	case DIK_A:
		if(kEvent.isHold) isBoostedSpeed = true;
		break;
	case DIK_LEFT:
		ax = -ACCELERATION_X_WALK;
		nx = -1;
		break;

	case DIK_RIGHT:
		ax = ACCELERATION_X_WALK;
		nx = 1;
		break;

	case DIK_DOWN:
		ChangeAction(MarioAction::CROUCH);
		break;

	case DIK_X:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		if (this->state.action == MarioAction::IDLE || this->state.action == MarioAction::WALK) {
			vy = -MARIO_JUMP_SPEED_Y;
			ChangeAction(MarioAction::JUMP);
		}
		break;
		
	case DIK_S:
		if(!kEvent.isHold && powerX < 6000) ChangeAction(MarioAction::JUMP);
		break;


	default:
		if (this->state.action == MarioAction::CROUCH) ChangeAction(MarioAction::IDLE);
		break;
	}

}


std::string CMario::GetAnimationId(MarioAction action) {
	std::string typeId;
	std::string actionId;
	switch (type)
	{	
	case MarioType::RED_SMALL:
		typeId = "ani-small-mario";
		break;
	case MarioType::RED_BIG:
		typeId = "ani-big-mario";
		break;
	case MarioType::RED_RACCON:
		typeId = "ani-raccoon-mario";
		break;
	case MarioType::FIRE:
		typeId = "ani-fire-mario";
		break;

	default:
		typeId = "ani-small-mario";
		break;
	}

	switch (action)
	{
	case MarioAction::IDLE:
		actionId = "idle";
		break;
	case MarioAction::WALK:
		actionId = "walk";
		break;
	case MarioAction::RUN:
		actionId = "run";
		break;
	case MarioAction::SPEEDUP:
		actionId = "speed-up";
		break;
	case MarioAction::JUMP:
		actionId = "jump";
		break;
	case MarioAction::CROUCH:
		actionId = "crouch";
		break;
	case MarioAction::FLY:
		actionId = "fly";
		break;
	case MarioAction::FALL:
		actionId = "fall";
		break;
	case MarioAction::SKID:
		actionId = "skid";
		break;
	case MarioAction::ATTACK:
		actionId = "attack";
		break;

	default:
		actionId = "idle";
		break;
	}
	
	return typeId + "-" + actionId;
}


/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CMario::ResetTempValues() {
	isBoostedSpeed = false;
	ax = 0;
}


void CMario::SetAction(MarioAction newAction, DWORD timeAction) {
	Vector2 oldBBox = GetBoundingBoxSize(type, state.action);
	Vector2 newBBox = GetBoundingBoxSize(type, newAction);
	x -= newBBox.x - oldBBox.x;
	y -= newBBox.y - oldBBox.y;
	
	state.action = newAction;
	state.beginAction = GetTickCount();
	state.timeAction = timeAction;
}

void CMario::ChangeAction(MarioAction newAction) {
	if (GetTickCount() < state.beginAction + state.timeAction) return;
	state.timeAction = 0;

	switch (newAction)
	{
	case MarioAction::IDLE:
		/*if (action == MarioAction::IDLE || action == MarioAction::RUN || action == MarioAction::WALK
			|| action == MarioAction::FALL || action == MarioAction::CROUCH)*/
		SetAction(newAction);
		break;

	case MarioAction::WALK:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::RUN || state.action == MarioAction::CROUCH) 
			SetAction(newAction);
		break;

	case MarioAction::RUN:
		if (state.action == MarioAction::IDLE) SetAction(newAction);
		break;

	case MarioAction::SPEEDUP:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::RUN) SetAction(newAction);
		break;

	case MarioAction::JUMP:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::WALK || state.action == MarioAction::RUN
				|| state.action == MarioAction::CROUCH) {

			vy = -MARIO_JUMP_SPEED_Y * 1.20f;
			if(state.action != MarioAction::CROUCH) SetAction(newAction);
		}
		break;
	case MarioAction::CROUCH:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::WALK || state.action == MarioAction::RUN)
			SetAction(newAction);
		break;

	case MarioAction::FLY:
		if (powerX > 6000) {
			vy = -MARIO_FLY_SPEED_Y;
			SetAction(newAction);
		}	
		break;

	case MarioAction::FALL:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::RUN || state.action == MarioAction::WALK
			|| state.action == MarioAction::FLY || state.action == MarioAction::JUMP)
			SetAction(newAction);
		break;

	case MarioAction::SKID:
		if ((state.action == MarioAction::RUN || state.action == MarioAction::WALK) && state.action != MarioAction::FLY && state.action != MarioAction::JUMP
			&& state.action != MarioAction::CROUCH) {
			vx = -nx * VELOCITY_X_AFTER_SKID;

			SetAction(newAction, 300);
		}
			
		break;

	case MarioAction::ATTACK:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::WALK
			|| state.action == MarioAction::RUN) {
			SetAction(newAction, 300);
		}
		break;

	default:
		break;
	}
}
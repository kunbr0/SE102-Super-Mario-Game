#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Koopas.h"
#include "Portal.h"

CMario::CMario(float x, float y) : CGameObject()
{
	tag = ETag::MARIO;
	untouchable = 0;

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CMario::CollidedLeftRight(vector<LPCOLLISIONEVENT> coEvents){
	if (state.action == MarioAction::DIE) return;
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents[i];

		if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Goomba 
		{
			DebugOut(ToWSTR("Koopas - Left or Right\n").c_str());
			if (((CKoopas*)(e->obj))->GetState() == EEnemyState::LIVE) {
				SetAction(MarioAction::DIE);
			}
			else {
				if (e->nx < 0)
					((CKoopas*)e->obj)->BeingCollidedLeft(tag);
				else
					((CKoopas*)e->obj)->BeingCollidedRight(tag);
			}
			
			
		}
	}
}

void CMario::CollidedTop(vector<LPCOLLISIONEVENT> coEvents) {
	if (state.action == MarioAction::DIE) return;
	if (state.action != MarioAction::CROUCH) {
		ChangeAction(MarioAction::IDLE);
		if (powerX > 0 && abs(vx) < VELOCITY_X_MIN_FOR_RUN)
			powerX -= POWER_X_LOSE_IN_GROUND;
	}
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents[i];
		if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Goomba 
		{	
			vy -= VELOCITY_Y_AFTER_COLLIDE_TOP_ENEMY;

			DebugOut(ToWSTR("Koopas - Top\n").c_str());
			if (((CKoopas*)e->obj)->GetState() == EEnemyState::LIVE) {
				((CKoopas*)e->obj)->BeingCollidedTop(tag);
				
			}
			else {
				float eLeft, eTop, eRight, eBottom;
				((CKoopas*)e->obj)->GetBoundingBox(eLeft, eTop, eRight, eBottom);
				if(this->x < (eRight+eLeft)/2)
					((CKoopas*)e->obj)->BeingCollidedLeft(tag);
				else
					((CKoopas*)e->obj)->BeingCollidedRight(tag);
			}
			
		}
	}
}

void CMario::Collided() {}
void CMario::NoCollided() {
	if (vy > 0) ChangeAction(MarioAction::FALL);
}


void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state.action == MarioAction::DIE) return;

	float vxmax = 0;
	switch (state.movementX)
	{
	case EMovementX::WALK:
		vxmax = VELOCITY_X_MAX_WALK;
		break;
	case EMovementX::RUN:
		vxmax = VELOCITY_X_MAX_RUN;
		break;
	case EMovementX::SPEEDUP:
		vxmax = VELOCITY_X_MAX_SPEEDUP;
		break;
	default:
		vxmax = VELOCITY_X_MAX_WALK;
		break;
	}
	
	// Increase velocity if in limit
	if (abs(vx) < vxmax)
		vx += ax * dt;

	//DebugOut(ToWSTR(std::to_string(dt) + "\n").c_str());

	applyFriction();
	applyGravity();

	// Calculate dx, dy 
	CGameObject::Update(dt);

	UpdateWithCollision(coObjects);
	
	

	if (vx != 0) {
		if (abs(vx) < VELOCITY_X_MIN_FOR_RUN && state.action != MarioAction::CROUCH) {
			ChangeAction(MarioAction::WALK);
		}
		else {
			
			if (powerX < POWER_X_MIN_FOR_SPEEDUP) {
				powerX += abs(dx) * POWER_X_RATIO_GAIN;
				ChangeAction(MarioAction::RUN);
			}
			else {
				powerX = POWER_X_WHEN_GETTING_SPEEDUP;
				ChangeAction(MarioAction::SPEEDUP);
			}
				
		}
			
	}

	// SKID
	if (vx * nx < 0 && abs(vx) > VELOCITY_X_MIN_FOR_SKID) {
		ChangeAction(MarioAction::SKID);
		
	}

	if(powerX > 0) powerX -= POWER_X_LOSE_ALWAYS;
	
	//DebugOut(ToWSTR(std::to_string((float)vxmax) + "\n").c_str());
	

	ResetTempValues(); // Set all temp values to initial value;
}





void CMario::Render(Vector2 finalPos) {
	if (state.action == MarioAction::DIE) return;

	std::vector<MarioAction> animationFlipY = {MarioAction::SKID};
	
	ChangeRenderAnimation(GetAnimationId(state.action));

	for (int i = 0; i < animationFlipY.size(); i++) {
		if (state.action == animationFlipY[i]) {
			SRenderAnimation ani;
			ani.AnimationID = GetAnimationId(state.action);
			ani.isFlipY = true;
			ChangeRenderAnimation(ani);
		}
	}

	CAnimations::GetInstance()->Get(renderAnimation.AnimationID)->Render(finalPos, 255, renderAnimation.isFlipY ? !(nx == 1 ? false : true) : (nx == 1 ? false : true));	
	//RenderBoundingBox(finalPos);

	
};

Vector2 CMario::GetBoundingBoxSize(MarioType mType, MarioAction mAction) {
	if(mAction == MarioAction::CROUCH) return Vector2(MARIO_SMALL_BBOX_WIDTH, MARIO_SMALL_BBOX_HEIGHT+10);
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
		if (kEvent.isHold)
			state.movementX = EMovementX::RUN;
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
		if (kEvent.isHold) ChangeAction(MarioAction::CROUCH);
		break;

	//case DIK_X:
	//	// TODO: need to check if Mario is *current* on a platform before allowing to jump again
	//	if (this->state.action == MarioAction::IDLE || this->state.action == MarioAction::WALK) {
	//		vy = -MARIO_JUMP_SPEED_Y;
	//		ChangeAction(MarioAction::JUMP);
	//	}
	//	break;
		
	case DIK_S:
		if(!kEvent.isHold) 
			ChangeAction(MarioAction::JUMP);
		else
			ChangeAction(MarioAction::HIGH_JUMP);
		break;


	default:
		if (this->state.action == MarioAction::CROUCH) 
			ChangeAction(MarioAction::IDLE);
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
	case MarioAction::HIGH_JUMP:
		actionId = "jump";
		break;
	case MarioAction::CROUCH:
		actionId = "crouch";
		break;
	case MarioAction::FLY:
		actionId = "fly";
		break;
	case MarioAction::FALL_SLIGHTLY:
		actionId = "fall-slightly";
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
	state.movementX = EMovementX::WALK;
	ax = 0;
}


void CMario::SetAction(MarioAction newAction, DWORD timeAction) {
	
	if (GetTickCount64() < state.beginAction + state.timeAction) return ;

	Vector2 oldBBox = GetBoundingBoxSize(type, state.action);
	Vector2 newBBox = GetBoundingBoxSize(type, newAction);
	x -= newBBox.x - oldBBox.x;
	y -= newBBox.y - oldBBox.y;
	
	state.action = newAction;
	state.beginAction = GetTickCount64();
	state.timeAction = timeAction;
}

bool CMario::ChangeAction(MarioAction newAction, DWORD timeAction) {
	
	if (state.action == MarioAction::DIE) return false;

	switch (newAction)
	{
	case MarioAction::IDLE:
		/*if (action == MarioAction::IDLE || action == MarioAction::RUN || action == MarioAction::WALK
			|| action == MarioAction::FALL || action == MarioAction::CROUCH)*/
		SetAction(newAction, timeAction);
		break;

	case MarioAction::WALK:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::RUN || state.action == MarioAction::CROUCH) 
			SetAction(newAction, timeAction);
		break;

	case MarioAction::RUN:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::WALK || state.action == MarioAction::SPEEDUP) 
			SetAction(newAction, timeAction);
		break;

	case MarioAction::SPEEDUP:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::RUN) SetAction(newAction, timeAction);
		break;

	case MarioAction::JUMP:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::WALK || state.action == MarioAction::RUN
				|| state.action == MarioAction::CROUCH || state.action == MarioAction::SPEEDUP) {
			vy = -MARIO_JUMP_SPEED_Y;
			SetAction(newAction, timeAction);
			if(state.action != MarioAction::CROUCH) SetAction(newAction, timeAction);
		}
		break;

	case MarioAction::HIGH_JUMP:
		if (state.action == MarioAction::JUMP && vy > -MARIO_JUMP_SPEED_Y * 0.6f && vy < -MARIO_JUMP_SPEED_Y * 0.55) {

			vy = -MARIO_JUMP_SPEED_Y;
			SetAction(newAction, timeAction);
		}
		break;

	case MarioAction::CROUCH:
		if ((state.action == MarioAction::IDLE || state.action == MarioAction::WALK || state.action == MarioAction::RUN) && type != MarioType::RED_SMALL)
			SetAction(newAction, timeAction);
		break;

	case MarioAction::FLY:
		if (powerX > 6000) {
			if(vy > -MARIO_FLY_SPEED_Y)
				vy = -MARIO_FLY_SPEED_Y;
			SetAction(newAction, timeAction);
		}	
		break;

	case MarioAction::FALL:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::RUN || state.action == MarioAction::WALK
			|| state.action == MarioAction::FLY || state.action == MarioAction::JUMP || state.action == MarioAction::FALL_SLIGHTLY
			|| state.action == MarioAction::HIGH_JUMP)
			SetAction(newAction, timeAction);
		break;

	case MarioAction::FALL_SLIGHTLY:
		if (state.action == MarioAction::FALL || state.action == MarioAction::FALL_SLIGHTLY) {
			if(vy > 0) vy = 0;
			SetAction(newAction, timeAction);
		}
			
		break;

	case MarioAction::SKID:
		if ((state.action == MarioAction::RUN || state.action == MarioAction::WALK || state.action == MarioAction::SPEEDUP) && state.action != MarioAction::FLY && state.action != MarioAction::JUMP
			&& state.action != MarioAction::CROUCH) {
			vx = -nx * VELOCITY_X_AFTER_SKID;

			SetAction(newAction, timeAction);
		}
			
		break;

	case MarioAction::ATTACK:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::WALK
			|| state.action == MarioAction::RUN) {
			SetAction(newAction, timeAction);
		}
		break;

	default:
		return false;
	}
	return true;
}
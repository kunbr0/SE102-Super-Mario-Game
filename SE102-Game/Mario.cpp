#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Enemy.h"

#include "Portal.h"

CMario::CMario(float x, float y) : CGameObject()
{

	untouchable = 0;

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CMario::BeingBouncedAfterJumpInTopEnemy() {
	vy = -VELOCITY_X_BEING_BOUNCED;
}

void CMario::BeingKilled() {
	if (state.action == MarioAction::EXPLODE) return;
	SetAction(MarioAction::DIE);
}

void CMario::TriggerLifeCycleOfActions() {
	
}

void CMario::CollidedTop(vector<LPCOLLISIONEVENT>* coEvents) {
	if (state.action == MarioAction::DIE) return;
	if (state.action != MarioAction::CROUCH) {
		ChangeAction(MarioAction::IDLE);
		if (powerX > 0 && abs(vx) < VELOCITY_X_MIN_FOR_RUN)
			powerX -= POWER_X_LOSE_IN_GROUND;
	}
	
	CGameObject::CollidedTop(coEvents);

}
void CMario::CollidedBottom(vector<LPCOLLISIONEVENT>* coEvents) {
	if (state.action == MarioAction::DIE) return;
	CGameObject::CollidedBottom(coEvents);
}


void CMario::Collided() {}
void CMario::NoCollided() {
	if (vy > 0) ChangeAction(MarioAction::FALL);
}


void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	if (state.action == MarioAction::DIE) return;
	// Increase velocity if in limit
	if (abs(vx) < vxmax)
		vx += ax * dt;

	//DebugOut(ToWSTR(std::to_string(vx) + "\n").c_str());

	ApplyFriction();
	ApplyGravity();

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
		ChangeAction(MarioAction::SKID, 250);
		
	}

	if(powerX > 0) powerX -= POWER_X_LOSE_ALWAYS;
	
	//DebugOut(ToWSTR(std::to_string((float)vxmax) + "\n").c_str());
	

	ResetTempValues(); // Set all temp values to initial value;
}

void CMario::Render(Vector2 finalPos) {
	if (state.action == MarioAction::DIE) return;

	std::vector<MarioAction> animationFlipY = {MarioAction::SKID};
	
	ChangeRenderAnimation(GetAnimationIdFromState());

	for (int i = 0; i < animationFlipY.size(); i++) {
		if (state.action == animationFlipY[i]) {
			SRenderAnimation ani;
			ani.AnimationID = GetAnimationIdFromState();
			ani.isFlipY = true;
			ChangeRenderAnimation(ani);
		}
	}
	
	
	float l, t, r, b;

	GetBoundingBox(l, t, r, b);

	//RenderBoundingBox(Vector2(finalPos.x + (l-this->x), finalPos.y + (t-this->y)));

	if (boost.type == MarioBoost::UNTOUCHABLE && GetTickCount64() % 100 > 50) return;
	CAnimations::GetInstance()->Get(renderAnimation.AnimationID)->Render(finalPos, Vector2(nx*(renderAnimation.isFlipY ? -1 : 1),ny), 255);

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
	if (state.action == MarioAction::EXPLODE) return;
	switch (kEvent.key)
	{
	case DIK_A:
		if (kEvent.isHold) {
			state.movementX = EMovementX::RUN;
			vxmax = VELOCITY_X_MAX_RUN;
		}
		break;
	case DIK_LEFT:
		ax = -ACCELERATION_X_WALK * ( 1 + (int)state.movementX*ACCELERATION_X_RUN_GREATER_RATIO);
		nx = -1;
		break;

	case DIK_RIGHT:
		ax = ACCELERATION_X_WALK * (1 + (int)state.movementX * ACCELERATION_X_RUN_GREATER_RATIO);
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


std::string CMario::GetAnimationIdFromState() {
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

	switch (state.action)
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
	case MarioAction::EXPLODE:
		typeId = "ani-mario";
		actionId = "damaged";
		break;

	default:
		actionId = "idle";
		break;
	}
	
	return typeId + "-" + actionId;
}


void CMario::Reset()
{
	
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CMario::ResetTempValues() {
	state.movementX = EMovementX::WALK;
	ax = 0;
	vxmax = VELOCITY_X_MAX_WALK;
}

void CMario::SetBoost(MarioBoost boostType, int beginBoost, int timeBoost) {
	if (GetTickCount64() < boost.beginBoost + boost.timeBoost) return;
	boost.type = boostType;
	boost.beginBoost = beginBoost;
	boost.timeBoost = timeBoost;
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
	if (state.action == MarioAction::EXPLODE) {
		SetBoost(MarioBoost::UNTOUCHABLE, state.beginAction, timeAction * 3);
	}
	else {
		SetBoost(); // SetBoost to default.
	}
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
			vy = -MARIO_JUMP_SPEED_Y * (1 + (vx > VELOCITY_X_MAX_WALK ? vx : 0));
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
			if(state.action == MarioAction::FLY)
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
			vy = 0;
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
	case MarioAction::EXPLODE:
		SetAction(newAction, 1000);
		break;
	case MarioAction::ATTACK:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::WALK
			|| state.action == MarioAction::RUN || state.action == MarioAction::JUMP || state.action == MarioAction::HIGH_JUMP) {
			SetAction(newAction, timeAction);
		}
		break;

	default:
		return false;
	}
	return true;
}
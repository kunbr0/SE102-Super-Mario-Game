#pragma once
#include "GameObject.h"
#include "MarioConsts.h"
#include "AnimationSet.h"
#include "Game.h"



enum class MarioType
{
	RED_SMALL,
	RED_BIG,
	RED_RACCON,
	FIRE
};

enum class MarioAction

{
	IDLE,
	WALK,
	RUN,
	SPEEDUP,
	JUMP,
	CROUCH,
	FLY,
	FALL,
	SKID,
	ATTACK
};

struct SMarioState {
	MarioAction action = MarioAction::IDLE;
	DWORD beginAction = 0;
	DWORD timeAction = 0; // How long the state is.
};

class CMario : public CGameObject
{

protected:
	float start_x;			// initial position of Mario at scene
	float start_y;
	

	

	MarioType type;

	int untouchable;
	DWORD untouchable_start;

	SMarioState state; // 0: isStandingInSomeThing
	
	bool isBoostedSpeed = false;
	bool isFlying = false;
	


public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render(Vector2 finalPos);

	virtual void ProcessKeyboard(SKeyboardEvent kEvent);


	virtual void SetAction(MarioAction newAction, DWORD timeAction = 0);
	virtual void ChangeAction(MarioAction newAction);


	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	std::string GetAnimationId(MarioAction action);

	void Reset();

	void ResetTempValues();

	Vector2 GetBoundingBoxSize(MarioType mType, MarioAction mAction);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
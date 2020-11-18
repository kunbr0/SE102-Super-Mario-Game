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

enum class MarioBoost {
	NONE,
	UNTOUCHABLE
};

enum class MarioAction

{
	IDLE,
	WALK,
	RUN,
	SPEEDUP,
	JUMP,
	HIGH_JUMP,
	CROUCH,
	FLY,
	FALL,
	FALL_SLIGHTLY,
	SKID,
	ATTACK,
	DIE,
	EXPLODE,
	
};

enum class EMovementX {
	WALK,
	RUN,
};

struct SMarioState {
	MarioAction action = MarioAction::IDLE;
	int beginAction = 0;
	int timeAction = 0; // How long the state is.
	EMovementX movementX = EMovementX::WALK;
};

struct SMarioBoost {
	MarioBoost type;
	int beginBoost = 0;
	int timeBoost = 0;
};

class CMario : public CGameObject
{

protected:
	float start_x;			// initial position of Mario at scene
	float start_y;
	
	float vxmax;
	float vymax;
	

	MarioType type;

	bool untouchable;
	DWORD untouchable_start;

	SMarioState state; // 0: isStandingInSomeThing
	SMarioBoost boost;
	//bool isBoostedSpeed = false;
	
	


public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL) override;
	virtual void Render(Vector2 finalPos) override;

	virtual void ProcessKeyboard(SKeyboardEvent kEvent);

	virtual void CollidedTop(vector<LPCOLLISIONEVENT>*) override;
	virtual void CollidedBottom(vector<LPCOLLISIONEVENT>*) override;
	virtual void Collided() override;
	

	virtual void NoCollided() override;


	virtual void BeingKilled();
	virtual void BeingBouncedAfterJumpInTopEnemy();


	virtual MarioType GetType() { return type; }

	virtual void SetAction(MarioAction newAction, DWORD timeAction = 0);
	virtual MarioAction GetAction() { return state.action; }
	virtual bool ChangeAction(MarioAction newAction, DWORD timeAction = 0);


	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	std::string GetAnimationIdFromState();

	void Reset();
	void SetBoost(MarioBoost = MarioBoost::NONE, int = 0, int = 0);
	void TriggerLifeCycleOfActions();

	void ResetTempValues();

	virtual Vector2 GetBoundingBoxSize(MarioType mType, MarioAction mAction);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
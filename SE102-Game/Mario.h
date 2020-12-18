#pragma once
#include "GameObject.h"
#include "MarioConsts.h"
#include "AnimationSet.h"
#include "Game.h"
#include <unordered_map>

#define UNTOUCHABLE_TIME		3000

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
	HIGH_JUMP,
	CROUCH,
	FLY,
	FALL,
	HOLD,
	FALL_SLIGHTLY,
	SKID,
	ATTACK,
	KICK,
	DIE,
	EXPLODE,
	UPGRADE_LV,
	GETTING_INTO_THE_HOLE
};

struct SGetIntoTheHole {
	Vector2 distance = Vector2(0,50);
	DWORD time = 1500;
	const DWORD totalTime = 1500;
	CallbackType OnFinish = nullptr;
};

struct SMarioState {
	MarioAction action = MarioAction::IDLE;
	int beginAction = 0;
	int timeAction = 0; // How long the state is.
	
};



struct SUntouchable {
	bool isUntouchable = false;
	int remainingTime = 3000;
};

class CMario : public CGameObject
{

protected:
	float start_x;			// initial position of Mario at scene
	float start_y;
	
	SGetIntoTheHole getIntoTheHole;
	Vector2 deltaRender = Vector2(0, 0);

	SUntouchable untouchable;

	MarioType type;

	SMarioState state; 
	
	std::unordered_map<int, bool> holdingKeys;
	


public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL) override;
	virtual void Render(Vector2 finalPos) override;

	
	//void GetIntoTheHole(Vector2, CallbackType = nullptr);
	
	
	void GetIntoTheHole(Vector2, CallbackType = nullptr);

	virtual void ProcessKeyboard(SKeyboardEvent kEvent);
	
	void BeginUntouchable();

	virtual void CollidedTop(vector<LPCOLLISIONEVENT>*) override;
	virtual void CollidedBottom(vector<LPCOLLISIONEVENT>*) override;
	virtual void Collided(vector<LPCOLLISIONEVENT>*) override;
	
	virtual void BeingCollided(LPGAMEOBJECT) override;

	virtual void NoCollided() override;

	bool IsReadyToChangeAction();

	virtual void BeingKilled();
	virtual void BeingBouncedAfterJumpInTopEnemy();

	bool isHoldingKey(int keyCode) { return holdingKeys[keyCode]; }

	virtual MarioType GetType() { return type; }

	virtual void SetAction(MarioAction newAction, DWORD timeAction = 0);
	virtual MarioAction GetAction() { return state.action; }
	virtual bool ChangeAction(MarioAction newAction, DWORD timeAction = 0);


	

	std::string GetAnimationIdFromState();

	void Reset();
	
	void TriggerLifeCycleOfActions();

	void ResetTempValues();

	virtual Vector2 GetBoundingBoxSize();
	virtual Vector2 GetBoundingBoxSize(MarioType mType, MarioAction mAction);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
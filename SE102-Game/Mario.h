#pragma once
#include "GameObject.h"
#include "MarioConsts.h"
#include "AnimationSet.h"



class CMario : public CGameObject
{

protected:
	float start_x;			// initial position of Mario at scene
	float start_y;
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
		JUMP,
		FLY,
		FALL,
		SKID,
		THROW
	};

	

	MarioType type;

	int untouchable;
	DWORD untouchable_start;

	MarioAction action; // 0: isStandingInSomeThing
	
	bool isBoostedSpeed = false;
	bool isFlying = false;
	
	string ani;
	std::string isShowingSpecialAni;

public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render(Vector2 finalPos);

	virtual void SetState(int state);

	virtual void ChangeAction(MarioAction newAction);


	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	std::string GetAnimationId(MarioAction action);

	void Reset();

	void ResetTempValues();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
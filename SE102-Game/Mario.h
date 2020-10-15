#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.15f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400

#define MARIO_ANI_BIG_IDLE_RIGHT		"ani-big-mario-idle"
#define MARIO_ANI_BIG_IDLE_LEFT			"ani-big-mario-idle"
#define MARIO_ANI_SMALL_IDLE_RIGHT		"ani-small-mario-idle"
#define MARIO_ANI_SMALL_IDLE_LEFT		"ani-small-mario-idle"

#define MARIO_ANI_BIG_WALKING_RIGHT			"ani-big-mario-walk"
#define MARIO_ANI_BIG_WALKING_LEFT			"ani-big-mario-walk"
#define MARIO_ANI_SMALL_WALKING_RIGHT		"ani-small-mario-walk"
#define MARIO_ANI_SMALL_WALKING_LEFT		"ani-small-mario-walk"

#define MARIO_ANI_DIE				8

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000

#include "AnimationSet.h"

class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	bool isStandingInSomething = false;
	float start_x;			// initial position of Mario at scene
	float start_y;
public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }

	int GetLevel() { return level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }


	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
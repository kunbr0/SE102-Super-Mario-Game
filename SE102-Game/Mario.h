#pragma once
#include "GameObject.h"
#include "MarioConsts.h"
#include "AnimationSet.h"

class CMario : public CGameObject
{
	float start_x;			// initial position of Mario at scene
	float start_y;

	// Type
	int type = 2; // 1: Red Mario, 2 Fire Mario
	int level;
	int untouchable;
	DWORD untouchable_start;
	bool isStandingInSomething = false;
	int status = 0;

public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);

	int GetType() { return type; }
	void SetType(int t) { type = t; }

	int GetLevel() { return level; }
	void SetLevel(int l) { level = l; }

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }


	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
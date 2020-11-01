#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED		0.09f;

#define GOOMBA_BBOX_WIDTH			48
#define GOOMBA_BBOX_HEIGHT			77
#define GOOMBA_BBOX_CROUCH_WIDTH	48
#define GOOMBA_BBOX_CROUCH_HEIGHT	40

#define GOOMBA_STATE_WALKING		100
#define GOOMBA_STATE_CROUCHING		200
#define MARGIN_STANDING_AREA		20


#define KOOPAS_ANI_WALKING "ani-red-koopa-troopa-move"
#define KOOPAS_ANI_CROUCH "ani-red-koopa-troopa-crouch"



class CKoopas : public CGameObject
{

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render(Vector2 finalPos);
	int state = 1;
	LPGAMEOBJECT standingObject = NULL;
	Vector2 stadingScope;

public:
	CKoopas(float x, float y);
	virtual void SetState(int state);
	virtual void CollideLeft(vector<LPCOLLISIONEVENT>);
	virtual void CollideTop(vector<LPCOLLISIONEVENT>);
	virtual void CollideRight(vector<LPCOLLISIONEVENT>);
	virtual void CollideBottom(vector<LPCOLLISIONEVENT>) {};
	virtual void Collided() {};
	void ChangeDirection();
	void Kill();
};
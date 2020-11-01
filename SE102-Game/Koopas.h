#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED 0.05f;

#define GOOMBA_BBOX_WIDTH 48
#define GOOMBA_BBOX_HEIGHT 77
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING	100
#define GOOMBA_STATE_CROUCHING	200

#define KOOPAS_ANI_WALKING "ani-red-koopa-troopa-move"
#define KOOPAS_ANI_CROUCH "ani-red-koopa-troopa-crouch"



class CKoopas : public CGameObject
{

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render(Vector2 finalPos);
	int state = 1;

public:
	CKoopas(float x, float y);
	virtual void SetState(int state);
	void Kill();
};
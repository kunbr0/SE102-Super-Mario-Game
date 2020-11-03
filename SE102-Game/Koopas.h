#pragma once
#include "Enemy.h"

#define GOOMBA_WALKING_SPEED		0.09f;

#define GOOMBA_BBOX_WIDTH			48
#define GOOMBA_BBOX_HEIGHT			77
#define GOOMBA_BBOX_CROUCH_WIDTH	48
#define GOOMBA_BBOX_CROUCH_HEIGHT	40

#define GOOMBA_STATE_WALKING		100
#define GOOMBA_STATE_CROUCHING		200
#define MARGIN_STANDING_AREA		0


#define KOOPAS_ANI_WALKING "ani-red-koopa-troopa-move"
#define KOOPAS_ANI_CROUCH "ani-red-koopa-troopa-crouch"



class CKoopas : public CEnemy
{

	

	LPGAMEOBJECT standingObject = NULL;
	Vector2 stadingScope;

public:
	CKoopas(float x, float y);
	virtual void SetState(EEnemyState state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render(Vector2 finalPos);

	virtual void CollidedLeft(vector<LPCOLLISIONEVENT>);
	virtual void CollidedTop(vector<LPCOLLISIONEVENT>);
	virtual void CollidedRight(vector<LPCOLLISIONEVENT>);
	virtual void CollidedBottom(vector<LPCOLLISIONEVENT>) {};
	virtual void Collided() {};

	std::string GetRenderAnimationId(EEnemyState);


	void ChangeDirection();

	void BeingCollided(ETag);
	void BeingCollidedTop(ETag);
	void BeingCollidedLeft(ETag);
	void BeingCollidedRight(ETag);
};
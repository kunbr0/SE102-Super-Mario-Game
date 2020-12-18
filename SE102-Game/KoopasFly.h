#pragma once
#include "Enemy.h"

#define KOOPASFLY_WALKING_SPEED		0.09f;

#define KOOPASFLY_BBOX_WIDTH			48
#define KOOPASFLY_BBOX_HEIGHT			77
#define KOOPASFLY_BBOX_CROUCH_WIDTH		48
#define KOOPASFLY_BBOX_CROUCH_HEIGHT	40




#define KOOPASFLY_ANI_WALKING "ani-green-koopa-paratroopa-fly"
#define KOOPASFLY_ANI_CROUCH "ani-red-koopa-troopa-crouch"



class CKoopasFly : public CEnemy
{



public:
	CKoopasFly(float x, float y);

	float GetDefaultWalkingSpeed() override { return KOOPASFLY_WALKING_SPEED; }

	Vector2 GetBoundingBoxSize(EEnemyState) override;

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	
	void CollidedTop(vector<LPCOLLISIONEVENT>*) override;
	void CollidedBottom(vector<LPCOLLISIONEVENT>*) override {};
	void Collided(vector<LPCOLLISIONEVENT>*) override {};
	void BeingCollidedTopBottom(LPGAMEOBJECT) override;
	std::string GetAnimationIdFromState() override;



};
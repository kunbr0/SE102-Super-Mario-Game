#pragma once
#include "RedGoomba.h"

#define KOOPASFLY_WALKING_SPEED		0.09f;

#define KOOPASFLY_BBOX_WIDTH			48
#define KOOPASFLY_BBOX_HEIGHT			77
#define KOOPASFLY_BBOX_CROUCH_WIDTH		48
#define KOOPASFLY_BBOX_CROUCH_HEIGHT	40




#define KOOPASFLY_ANI_WALKING "ani-green-koopa-paratroopa-fly"
#define KOOPASFLY_ANI_CROUCH "ani-red-koopa-troopa-crouch"



class CKoopasFly : public CRedGoomba
{
	

public:
	CKoopasFly(float x, float y);

	virtual std::string GetAnimationIdFromState() override;
	void ProcessHasWingBehavior() override;
	Vector2 GetBoundingBoxSize(EEnemyState) override;

};
#pragma once
#include "Enemy.h"

#define GOOMBA_WALKING_SPEED 0.09f;

#define GOOMBA_BBOX_WIDTH				48
#define GOOMBA_BBOX_HEIGHT				45
#define GOOMBA_BBOX_WILL_DIE_HEIGHT		18

#define GOOMBA_BBOX_HEIGHT_DIE 9


#define GOOMBA_ANI_WALKING			"ani-goomba-walk"
#define GOOMBA_ANI_WILL_DIE			"ani-goomba-die"

#define GOOMBA_ANI_DIE				"ani-goomba-die"

class CGoomba : public CEnemy
{
	
public:
	CGoomba(float x, float y);

	float GetDefaultWalkingSpeed() override { return GOOMBA_WALKING_SPEED; }

	Vector2 GetBoundingBoxSize(EEnemyState) override;

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void CollidedTop(vector<LPCOLLISIONEVENT>*) override;
	void CollidedBottom(vector<LPCOLLISIONEVENT>*) override {};
	void Collided(vector<LPCOLLISIONEVENT>*) override {};

	
	void BeingCollidedTopBottom(LPGAMEOBJECT) override;

	

	void ChangeState(EEnemyState, DWORD = 0) override;

	std::string GetAnimationIdFromState() override;


};
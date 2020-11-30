#pragma once
#include "Enemy.h"

#define GOOMBA_WALKING_SPEED 0.09f;

#define GOOMBA_BBOX_WIDTH 48
#define GOOMBA_BBOX_HEIGHT 45
#define GOOMBA_BBOX_HEIGHT_DIE 9


#define GOOMBA_ANI_WALKING			"ani-goomba-walk"
#define GOOMBA_ANI_DIE				"ani-goomba-die"

class CGoomba : public CEnemy
{
	
public:
	CGoomba(float x, float y);

	float GetDefaultWalkingSpeed() override { return GOOMBA_WALKING_SPEED; }

	Vector2 GetBoundingBoxSize(EEnemyState) override;

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void CollidedLeftRight(vector<LPCOLLISIONEVENT>*) override;
	void CollidedTop(vector<LPCOLLISIONEVENT>*) override;
	void CollidedBottom(vector<LPCOLLISIONEVENT>*) override {};
	void Collided() override {};

	//void BeingCollided(ETag, Vector2);
	void BeingCollidedTop(LPGAMEOBJECT) override;
	void BeingCollidedTopBottom(LPGAMEOBJECT) override;

	void BeingCollidedLeftRight(LPGAMEOBJECT) override;
	void BeingCollided(LPGAMEOBJECT) override;
	//void BeingCollidedRight(ETag, Vector2);


	std::string GetAnimationIdFromState() override;


};
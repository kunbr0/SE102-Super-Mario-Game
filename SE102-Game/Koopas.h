#pragma once
#include "Enemy.h"

#define KOOPAS_WALKING_SPEED		0.09f;

#define KOOPAS_BBOX_WIDTH			48
#define KOOPAS_BBOX_HEIGHT			77
#define KOOPAS_BBOX_CROUCH_WIDTH	48
#define KOOPAS_BBOX_CROUCH_HEIGHT	40


#define KOOPAS_ANI_WALKING "ani-red-koopa-troopa-move"
#define KOOPAS_ANI_CROUCH "ani-red-koopa-troopa-crouch"



class CKoopas : public CEnemy
{



public:
	CKoopas(float x, float y);
	
	float GetDefaultWalkingSpeed() override { return KOOPAS_WALKING_SPEED; }

	Vector2 GetBoundingBoxSize(EEnemyState) override;
	
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void CollidedTop(vector<LPCOLLISIONEVENT>*) override;
	void CollidedBottom(vector<LPCOLLISIONEVENT>*) override {} ;
	void Collided() override {};

	//void BeingCollided(ETag, Vector2);
	void BeingCollidedTopBottom(LPGAMEOBJECT) override;
	
	//void BeingCollided(LPGAMEOBJECT) override;
	//void BeingCollidedRight(ETag, Vector2);


	std::string GetAnimationIdFromState() override;


	
};
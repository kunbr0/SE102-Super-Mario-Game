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



public:
	CKoopas(float x, float y);
	
	void ChangeState(EEnemyState newState);

	Vector2 GetBoundingBoxSize() override;
	
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void CollidedLeftRight(vector<LPCOLLISIONEVENT>*) override;
	void CollidedTop(vector<LPCOLLISIONEVENT>*) override;
	void CollidedBottom(vector<LPCOLLISIONEVENT>*) override {} ;
	void Collided() override {};

	//void BeingCollided(ETag, Vector2);
	void BeingCollidedTop(ETag, Vector2);
	void BeingCollidedLeftRight(ETag, Vector2);
	//void BeingCollidedRight(ETag, Vector2);


	std::string GetRenderAnimationId(EEnemyState);


	void Kick(Vector2 pos);
	
};
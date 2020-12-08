#pragma once
#include "GameObject.h"


#define FIRE_BULLET_WIDTH		30
#define FIRE_BULLET_HEIGHT		30
#define ANI_FIRE_BULLET			"ani-fire-bullet"
#define VELOCITY_X_FIRE_BULLET			0.028125f 
#define VELOCITY_Y_FIRE_BULLET			0.14f 
#define VELOCITY_Y_FIRE_BULLET_BOUNCE	0.485f 
#define INITIAL_DELTA_Y_FIRE_BULLET		10

#define DELTA_WIDTH_IN_LEFT_SIDE	80


class CFireBullet : public CGameObject
{
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	
	

public:
	CFireBullet(float x, float y, int nx);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render(Vector2 finalPos);

	void PrepareForShooting();
	void UpdatePos(Vector2 pos, int nx=1);


	virtual void CollidedLeft(vector<LPCOLLISIONEVENT>*) override;
	virtual void CollidedTop(vector<LPCOLLISIONEVENT>*) override;
	virtual void CollidedRight(vector<LPCOLLISIONEVENT>*) override;
	virtual void CollidedBottom(vector<LPCOLLISIONEVENT>*) override {};
	virtual void Collided(vector<LPCOLLISIONEVENT>*) override {};
};


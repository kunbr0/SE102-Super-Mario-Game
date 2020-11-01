#pragma once
#include "GameObject.h"


#define FIRE_BULLET_WIDTH		30
#define FIRE_BULLET_HEIGHT		30
#define ANI_FIRE_BULLET			"ani-fire-bullet"

class CFireBullet : public CGameObject
{
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	
	

public:
	CFireBullet(float x, float y, int nx);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render(Vector2 finalPos);

	void PrepareForShooting();
	void UpdatePos(Vector2 pos, int nx=1);


	virtual void CollideLeft(vector<LPCOLLISIONEVENT>) ;
	virtual void CollideTop(vector<LPCOLLISIONEVENT>);
	virtual void CollideRight(vector<LPCOLLISIONEVENT>) ;
	virtual void CollideBottom(vector<LPCOLLISIONEVENT>) {};
	virtual void Collided() {};
};


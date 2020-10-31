#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "SpriteManager.h"
#include "AnimationSet.h"
#include "PhysicsConsts.h"



#define ID_TEX_BBOX "-100"		// special texture to draw object bounding box

class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};

struct SCollisionResult {
	bool isCollided = false;
	float nx, ny = 0.0f;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
};

struct SPrevBoundingBox {
	float left, top, right, bottom = 0;
};

struct SRenderAnimation {
	std::string AnimationID;
	bool isFlipY = false;
};

class CGameObject
{
public:
	// Position
	float x;
	float y;

	// Velocity
	float vx;
	float vy;

	// Acceleration
	float ax;
	float ay;

	// Distance
	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	int powerX = 0;
	// Direction of Object ( Left , Right )
	int nx;


	DWORD dt;

	LPANIMATION_SET animation_set;

	bool isDisable = false;

	SRenderAnimation renderAnimation;
	SPrevBoundingBox prevBoundingBox;

public:
	// Con/Destructor
	CGameObject();
	~CGameObject();

	// Position 
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void SetPosition(float x, float y) { this->x = x, this->y = y; }

	// Speed
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	
	

	void ResetRenderAnimation();
	virtual void ChangeRenderAnimation(std::string newRenderAnimationID);
	virtual void ChangeRenderAnimation(SRenderAnimation newRenderAnimation);

	// BoundingBox
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	void RenderBoundingBox(Vector2 finalPos);

	// AnimationSet
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	// Collision
	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);

	// Physics
	void applyGravity();
	void applyFriction();
	SCollisionResult calcCollision(vector<LPGAMEOBJECT>* coObjects);
	void cleanAfterCalcCollision(SCollisionResult result);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render(Vector2 finalPos) = 0;

	
};


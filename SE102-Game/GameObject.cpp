#include <d3dx9.h>
#include <algorithm>


#include "debug.h"
#include "TextureManager.h"
#include "Game.h"
#include "GameObject.h"
#include "RectPlatform.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
		);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			if (dynamic_cast<CRectPlatform*>(coEvents[i]->obj)) {
				// Do not thing ( allow Mario go in X-direction )
			}
			else {
				min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
			}
			
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}


void CGameObject::RenderBoundingBox(Vector2 finalPos)
{
	
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	
	CGame::GetInstance()->Draw(finalPos, Vector2(0,0), bbox, rect, 222);
	
}

void CGameObject::applyGravity() {
	vy += ACCELERATION_Y_GRAVITY * dt;

}

void CGameObject::applyFriction() {
	if (vx > 0) {
		vx += -ACCELERATION_FRICTION*dt;
		if (vx < 0) vx = 0;
	}
	if (vx < 0) {
		vx += ACCELERATION_FRICTION*dt;
		if (vx > 0) vx = 0;
	}
}

SCollisionResult CGameObject::calcCollision(vector<LPGAMEOBJECT>* coObjects) {
	SCollisionResult result;
	
	//vector<LPCOLLISIONEVENT> coEventsResult;
	

	result.coEvents.clear();

	CalcPotentialCollisions(coObjects, result.coEvents);
	if (result.coEvents.size() == 0)
	{
		result.isCollided = false;
		x += dx;
		y += dy;
		
	}
	else
	{
		result.isCollided = true;
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(result.coEvents, result.coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		result.nx = nx;
		result.ny = ny;

		// block every object first!
		x += min_tx * dx + nx * 0.4f;

		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;

		if (ny != 0) {
			vy = 0;
		}

		

	}

	
	return result;
}

void CGameObject::cleanAfterCalcCollision(SCollisionResult result) {
	for (UINT i = 0; i < result.coEvents.size(); i++) delete result.coEvents[i];
}


void CGameObject::ResetRenderAnimation() {
	renderAnimation.AnimationID = "";
	renderAnimation.isFlipY = false;
}

void CGameObject::ChangeRenderAnimation(std::string newRenderAnimationID) {
	// Completed rendering last animation 
	ResetRenderAnimation();
	renderAnimation.AnimationID = newRenderAnimationID;
}

void CGameObject::ChangeRenderAnimation(SRenderAnimation newRenderAnimation) {
	// Completed rendering last animation 
	renderAnimation = newRenderAnimation;
}

CGameObject::~CGameObject()
{

}
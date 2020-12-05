#include <d3dx9.h>


#include <algorithm>


#include "debug.h"
#include "TextureManager.h"
#include "Game.h"
#include "GameObject.h"
#include "RectPlatform.h"
#include "Coin.h"
#define ANIMATIONID_BEING_DAMAGE		"ani-enemy-damaged"
#define ANIMATIONID_BONUS				"ani-mario-damaged"
#define DELTA_TO_APPLY_NEW_BASE_POSITION		500

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

	if (((ml + (mr-ml) >= sl) && (sl + (sr-sl) >= ml) 
		&& (mt + (mr-ml) >= st) && (st + (sr-sl) >= mt))) {
		coO->OnHadCollided(this);
	}

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
		//// Bounding Box of GameObject
		//float oLeft, oTop, oRight, oBottom;
		//this->GetBoundingBox(oLeft, oTop, oRight, oBottom);

		//// Bounding Box cua TargetObject
		//float tLeft, tTop, tRight, tBottom;
		//coObjects->at(i)->GetBoundingBox(tLeft, tTop, tRight, tBottom);

		//if (
		//	(oLeft >= tLeft && oLeft <= tRight) &&
		//	(oTop >= tTop && oTop <= oBottom) &&
		//	(oRight > oLeft && oBottom > oTop) // ensure that this object has collision box != 0,0,0,0
		//)
		//{
		//}
		//else {
		//	
		//}

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

void CGameObject::RenderMotionableAnimation(int totalStep) {

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

	
	//CGame::GetInstance()->Draw(finalPos, Vector2(0,0), bbox, rect, 222);
	CGame::GetInstance()-> DrawWithScaling(finalPos, Vector2(0,0), bbox, rect, 222);

}

void CGameObject::ApplyGravity() {
	vy += ACCELERATION_Y_GRAVITY * dt;

}

void CGameObject::ApplyFriction() {
	if (vx > 0) {
		vx += -ACCELERATION_FRICTION*dt;
		if (vx < 0) vx = 0;
	}
	if (vx < 0) {
		vx += ACCELERATION_FRICTION*dt;
		if (vx > 0) vx = 0;
	}
}

void CGameObject::ChangeBasePosition(Vector2 newPos) {
	if (abs(basePosition.x - newPos.x) > DELTA_TO_APPLY_NEW_BASE_POSITION) basePosition.x = newPos.x;
	if (abs(basePosition.y - newPos.y) > DELTA_TO_APPLY_NEW_BASE_POSITION) basePosition.y = newPos.y;
	
}

void CGameObject::CollidedLeftRight(vector<LPCOLLISIONEVENT>* coEvents) {
	for (UINT i = 0; i < coEvents->size(); i++)
		coEvents->at(i)->obj->BeingCollidedLeftRight(this);
}
void CGameObject::CollidedLeft(vector<LPCOLLISIONEVENT>* e) { CollidedLeftRight(e); };
void CGameObject::CollidedRight(vector<LPCOLLISIONEVENT>* e) { CollidedLeftRight(e); };
void CGameObject::CollidedTop(vector<LPCOLLISIONEVENT>* coEvents) {
	for (UINT i = 0; i < coEvents->size(); i++)
		coEvents->at(i)->obj->BeingCollidedTop(this);
};
void CGameObject::CollidedBottom(vector<LPCOLLISIONEVENT>* coEvents) {
	for (UINT i = 0; i < coEvents->size(); i++)
		coEvents->at(i)->obj->BeingCollidedBottom(this);
};

void CGameObject::UpdateNoCollision() {
	x += dx;
	y += dy;
}

void CGameObject::UpdateWithCollision(vector<LPGAMEOBJECT>* coObjects) {
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	
	
	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		NoCollided();

		if (dynamic_cast<CCoin*>(this)) {
			auto aa = this;
			int a = 9;

		}
	}
	else
	{
		
		if (dynamic_cast<CCoin*>(this)) {
			auto aa = this;
			int a = 9;

		}

		
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		Collided(&coEventsResult);

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		if(ny < 0)
			y += min_ty * dy + ny * 0.4f;

		if (nx != 0) { 
			vx = 0; 
			if (nx > 0) CollidedRight(&coEventsResult);
			else CollidedLeft(&coEventsResult);
		}

		else if (ny != 0) {
			vy = 0;
			if (ny > 0) CollidedBottom(&coEventsResult);
			else 
				CollidedTop(&coEventsResult);
		}

		

	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}


void CGameObject::RenderExtraEffect(Vector2 finalPos) {
	if (effect.type != EExtraEffect::NONE)
		CAnimations::GetInstance()->Get(CGameObject::GetRenderAnimationId(effect.type))->Render(
			Vector2(finalPos.x + (effect.initPosition.x - this->x), finalPos.y + (effect.initPosition.y - this->y)),
			Vector2(-nx, ny), 255);
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



void CGameObject::SwitchEffect(EExtraEffect eff) {
	SetEffect(eff, 500);
	effect.initPosition = Vector2(this->x, this->y);
}

void CGameObject::SetEffect(EExtraEffect newEffect, DWORD timeEffect) {
	if (GetTickCount64() < effect.timeEffect + effect.timeBegin) return;
	effect.timeBegin = GetTickCount64();
	effect.timeEffect = timeEffect;
	effect.type = newEffect;
}



std::string CGameObject::GetRenderAnimationId(EExtraEffect effctType) {
	switch (effctType)
	{
	case EExtraEffect::BEING_DAMAGED:
		return ANIMATIONID_BEING_DAMAGE;
	case EExtraEffect::BONUS:
		return ANIMATIONID_BONUS;
	default:

		return ANIMATIONID_BEING_DAMAGE;
	}
}

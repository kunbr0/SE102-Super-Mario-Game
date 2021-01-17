#include "MovablePlatform.h"
#include "SpriteManager.h"

CMovablePlatform::CMovablePlatform(float x, float y, float width, float height) : CRectPlatform(x,y,width,height) {
	
	
	isFalling = false;
}

void CMovablePlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (isFalling) {
		vx = 0; 
		vy = 0.15f;
	}
	else {
		vx = -0.05f;
		vy = 0;
	}
	CGameObject::Update(dt, coObjects);
	UpdateWithCollision(coObjects);
}

void CMovablePlatform::BeingCollidedTop(LPGAMEOBJECT obj) {
	isFalling = true;
}


void CMovablePlatform::Render(Vector2 finalPos) {
	CSprites::GetInstance()->Get("spr-platform-0")->DrawWithScaling(finalPos);
	CRectPlatform::Render(finalPos);
}
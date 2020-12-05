#include "MiniPortal.h"
#include "PlayScene.h"
#include "Mario.h"


CMiniPortal::CMiniPortal( Vector2 portalPosition, Vector2 portalSize, Vector2 portalTargetPos, Vector2 portalTargetSize, Vector2 portalLimit1, Vector2 portalLimit2) {

	this->x = portalPosition.x;
	this->y = portalPosition.y;
	this->portalSize = portalSize;
	this->targetPosition = portalTargetPos;
	this->targetSize = portalTargetSize;
	this->targetCameraLeffTopLimit = portalLimit1;
	this->targetCameraRightBottomLimit = portalLimit2;
}

void CMiniPortal::Collided(vector<LPCOLLISIONEVENT>* coEvents) {
	for (int i = 0; i < coEvents->size(); i++) {
		if (dynamic_cast<CMario*>(coEvents->at(i)->obj)) {
			if (((CMario*)(coEvents->at(i)->obj))->isHoldingKey(DIK_DOWN)) {
				((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->ChangeCameraArea(targetPosition + targetSize /2 , targetCameraLeffTopLimit, targetCameraRightBottomLimit);
			}
			return;
		}
	}
}

void CMiniPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	UpdateWithCollision(coObjects);
}

void CMiniPortal::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - portalSize.x / 2;
	top = y - portalSize.y / 2;
	right = x + portalSize.x / 2;
	bottom = y + portalSize.y / 2;
}
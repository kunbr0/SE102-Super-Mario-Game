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

void CMiniPortal::BeingCollided(LPGAMEOBJECT obj) {
	if (dynamic_cast<CMario*>(obj)) {
		if (((CMario*)(obj))->isHoldingKey(DIK_DOWN)) {
			((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->ChangeCameraArea(targetPosition + targetSize /2 , targetCameraLeffTopLimit, targetCameraRightBottomLimit);
		}
		return;
	}
}


void CMiniPortal::Render(Vector2 finalPos) {
	RenderBoundingBox(finalPos);
}


void CMiniPortal::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - portalSize.x / 2;
	top = y - portalSize.y / 2;
	right = x + portalSize.x / 2;
	bottom = y + portalSize.y / 2;
}
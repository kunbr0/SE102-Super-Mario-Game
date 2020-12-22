#include "EndSceneItem.h"
#include "Animation.h"
#include "SpriteManager.h"
#include "Mario.h"

CEndSceneItem::CEndSceneItem(Vector2 pos) {
	SetPosition(pos);
	isTemp = true;
}

void CEndSceneItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs) {
	CGameObject::Update(dt, coObjs);
	UpdateWithCollision(coObjs);
	if (claimBonusEffect.isActive) {
		claimBonusEffect.remainingTime -= dt;
		deltaRender.x = claimBonusEffect.totalMoveDistance.x * (1 - (claimBonusEffect.remainingTime * 1.0f) / claimBonusEffect.totalTime);
		deltaRender.y = claimBonusEffect.totalMoveDistance.y * (1 - (claimBonusEffect.remainingTime * 1.0f) / claimBonusEffect.totalTime);
		if (claimBonusEffect.remainingTime <= 0) claimBonusEffect.isActive = false;
	}
}

void CEndSceneItem::Render(Vector2 finalPos) {
	if (claimBonusEffect.isActive == false) {
		CAnimations::GetInstance()->Get("ani-end-scene-world1-1")->Render(finalPos + deltaRender);
	}
	else {
		CAnimations::GetInstance()->Get("ani-end-scene-world1-1")->Render(finalPos + deltaRender);
	}
	
}

void CEndSceneItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - size.x / 2;
	top = y - size.y / 2;
	right = x + size.x / 2;
	bottom = y + size.y / 2;
}


void CEndSceneItem::Collided(vector<LPCOLLISIONEVENT>* coEvents) {
	for (int i = 0; i < coEvents->size(); i++) {
		if (dynamic_cast<CMario*>(coEvents->at(i)->obj)) {
			claimBonusEffect.isActive = true;
			claimBonusEffect.remainingTime = claimBonusEffect.totalTime;
		}
	}
}
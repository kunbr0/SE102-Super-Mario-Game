#include "EndSceneItem.h"
#include "Animation.h"
#include "SpriteManager.h"
#include "Mario.h"

#define SHOW_TIME_CARD			1000
#define TOTAL_QUANTITY_OF_CARD		3

CEndSceneItem::CEndSceneItem(Vector2 pos) {
	SetPosition(pos);
	isTemp = true;
}

void CEndSceneItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs) {
	ChangeCardId(dt);
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
		CSprites::GetInstance()->Get("spr-misc-card-" + std::to_string(cardId))->DrawWithScaling(finalPos + deltaRender);
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


void CEndSceneItem::Collided(LPGAMEOBJECT obj) {
	if (dynamic_cast<CMario*>(obj)) {
		claimBonusEffect.isActive = true;
		claimBonusEffect.remainingTime = claimBonusEffect.totalTime;
		((CMario*)obj)->vx = 0;
		((CMario*)obj)->vy = 0;
		((CMario*)obj)->ChangeFinishStep(1);
		CGame::GetInstance()->GetCurrentScene()->PushToCards(cardId);
	}
}

void CEndSceneItem::ChangeCardId(DWORD dt) {
	cardIdRemainingTime -= dt;
	if (cardIdRemainingTime <= 0) {
		cardId++;
		cardIdRemainingTime = SHOW_TIME_CARD;
		if (cardId >= TOTAL_QUANTITY_OF_CARD) cardId = 0;
	}
}
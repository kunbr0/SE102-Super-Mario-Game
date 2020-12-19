#include "EndSceneItem.h"
#include "Animation.h"
#include "SpriteManager.h"

CEndSceneItem::CEndSceneItem(Vector2 pos) {
	SetPosition(pos);
	isTemp = true;
}


void CEndSceneItem::Render(Vector2 finalPos) {
	if (claimBonusEffect.isActive == false) {
		CAnimations::GetInstance()->Get("ani-end-scene-world1-1")->Render(finalPos);
	}
	else {
		//CSprites::GetInstance()->Get("spr-end-scene-flower-0")->DrawWithScaling()
	}
	
}

void CEndSceneItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - size.x / 2;
	top = y - size.y / 2;
	right = x + size.x / 2;
	bottom = y + size.y / 2;
}

#include "BoundingCoinEffect.h"
#include "PlayScene.h"
#include "AddingPointEffect.h"

#define BOUNDING_COIN_ANIMATIONID	"ani-coin-in-question-block"


std::string CBoundingCoinEffect::GetAnimationId() {
	return BOUNDING_COIN_ANIMATIONID;
}

void CBoundingCoinEffect::OnFinish() {
	((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEffects(new CAddingPointEffect(initialPosition, Vector2(0, -0.44)));
}
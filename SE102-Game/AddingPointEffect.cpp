#include "AddingPointEffect.h"
#include "Game.h"

#define ADDING_POINT_100	"ani-100"


std::string CAddingPointEffect::GetAnimationId() {
	return ADDING_POINT_100;
}

void CAddingPointEffect::OnFinish() {
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->AdjustScore(100);
}

void CAddingPointEffect::Update(DWORD dt) {
	if (!isActive) return;
	currentPosition.y += deltaPosition.y * dt;
	deltaPosition.y += DEFAULT_CHANGE_DELTA_POSITION_Y * dt;
	if (deltaPosition.y > 0) {
		OnFinish();
		isActive = false;
	}
}


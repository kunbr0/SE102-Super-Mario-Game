#include "AddingPointEffect.h"
#include "Game.h"

#define ADDING_POINT_100	"ani-100"
#define DELTA_CHANGE_DELTA_POSITION_Y_ADDINGPOINT	0.0001f

std::string CAddingPointEffect::GetAnimationId() {
	return ADDING_POINT_100;
}

void CAddingPointEffect::OnFinish() {
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->AdjustScore(100);
}

void CAddingPointEffect::Update(DWORD dt) {
	if (!isActive) return;
	currentPosition.y += deltaPosition.y * dt;
	deltaPosition.y += DELTA_CHANGE_DELTA_POSITION_Y_ADDINGPOINT * dt;
	if (deltaPosition.y > 0) {
		OnFinish();
		isActive = false;
	}
}


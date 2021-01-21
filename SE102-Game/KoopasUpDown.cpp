#include "KoopasUpDown.h"
#define TIME_OF_ONE_DIRECTION			3000
CKoopasUpDown::CKoopasUpDown(float x, float y) : CKoopasFly(x, y) {
	initPos = Vector2(x, y);
	remainingTimeToChangeDirection = TIME_OF_ONE_DIRECTION / 2;
	direction = 1;
}


std::string CKoopasUpDown::GetAnimationIdFromState() {
	switch (state.type)
	{
	case EEnemyState::LIVE: {
		return "ani-red-koopa-paratroopa-fly";
	}

	case EEnemyState::LIVE1:
		return "ani-red-koopa-troopa-move";


	case EEnemyState::ONESHOTDIE:
		return "ani-red-koopa-troopa-move";
	case EEnemyState::WILL_DIE:
		return "ani-red-koopa-troopa-crouch";
	default:
		return "ani-red-koopa-troopa-move";
	}
}

void CKoopasUpDown::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state.type == EEnemyState::LIVE) {
		remainingTimeToChangeDirection -= dt;
		if (remainingTimeToChangeDirection < 0) {
			remainingTimeToChangeDirection = TIME_OF_ONE_DIRECTION;
			direction *= -1;
		}
		y += 0.065f * dt * direction;
	}
	else if (state.type == EEnemyState::LIVE1) {
		ApplyGravity();
		CGameObject::Update(dt);
		y += dy;
	}
	
}
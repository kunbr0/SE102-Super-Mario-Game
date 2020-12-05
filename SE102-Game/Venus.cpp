#include "Venus.h"



CVenus::CVenus(Vector2 initPos, Vector2 size) : CEnemy() {
	this->initPos = initPos;
	this->size = size;
	this->x = initPos.x;
	this->y = initPos.y;
	ChangeState(EVenusState::HEADDOWN);
}

void CVenus::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	int a = 0;
	if (state.type == EVenusState::HEADUP) {
		y = initPos.y + ((float)(state.timeRemaining *1.0f / VENUS_TIME_DURATION_PER_STATE)) * size.y;
		ChangeState(EVenusState::HEADDOWN);
	}
	else if (state.type == EVenusState::HEADDOWN) {
		y = initPos.y + (1-(float)(state.timeRemaining * 1.0f / VENUS_TIME_DURATION_PER_STATE)) * size.y;
		ChangeState(EVenusState::HEADUP);
	}
	state.timeRemaining -= dt;
	//DebugOut(ToWSTR(std::to_string((int)state.timeRemaining) + "\n").c_str());
}




void CVenus::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - size.x / 2;
	top = y - size.y / 2;
	right = x + size.x / 2;
	bottom = y + size.y / 2;
}

std::string CVenus::GetAnimationIdFromState() {
	switch (state.type) {
	case EVenusState::HEADUP:
		return "ani-red-venus-fire-trap-headup";
	case EVenusState::HEADDOWN:
		return "ani-red-venus-fire-trap-headdown";
	default:
		return "ani-red-venus-fire-trap-headup";
	}
	
}

void CVenus::ChangeState(EVenusState newState) {
	if (state.timeRemaining <= 0) {
		this->state.type = newState;
		this->state.timeRemaining = VENUS_TIME_DURATION_PER_STATE;
	}
}
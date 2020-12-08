#include "Venus.h"
#include "PlayScene.h"



CVenus::CVenus(Vector2 initPos, Vector2 size) : CEnemy() {
	this->initPos = initPos;
	this->size = size;
	this->x = initPos.x;
	this->y = initPos.y;
	ChangeState(EVenusState::HEADDOWN_IDLE);
	y = initPos.y + size.y;
}

bool CVenus::ShootBullet() {
	vector<LPGAMEOBJECT> bullets = ((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetEnemyBullets();
	for (int i = 0; i < bullets.size(); i++) {
		if(dynamic_cast<CFireBullet*>(bullets.at(i))){
			if (bullets.at(i)->isDisable) {
				bullets.at(i)->vy = VELOCITY_Y_FIRE_BULLET * 0.3;
				bullets.at(i)->x = this->x;
				bullets.at(i)->y = this->y + INITIAL_DELTA_Y_FIRE_BULLET;
				bullets.at(i)->nx = nx;
				bullets.at(i)->isDisable = false;
				return true;
			}
		}
		
	}
	return false;
}

void CVenus::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	int playerX = ((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetPlayer()->GetPosition().x;
	if (playerX > this->x) nx = 1; else nx = -1;

	int a = 0;
	if (state.type == EVenusState::HEADINGUP) {
		y = initPos.y + ((float)(state.timeRemaining *1.0f / VENUS_TIME_DURATION_HEADING_DOWNUP)) * size.y;
		ChangeState(EVenusState::HEADUP_IDLE);
	}
	else if (state.type == EVenusState::HEADINGDOWN) {
		y = initPos.y + (1-(float)(state.timeRemaining * 1.0f / VENUS_TIME_DURATION_HEADING_DOWNUP)) * size.y;
		ChangeState(EVenusState::HEADDOWN_IDLE);
	}
	else if (state.type == EVenusState::HEADUP_IDLE) {
		y = initPos.y;
		ChangeState(EVenusState::HEADINGDOWN);
	}
	else if (state.type == EVenusState::HEADDOWN_IDLE) {
		y = initPos.y + size.y;
		ChangeState(EVenusState::HEADINGUP);
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
	case EVenusState::HEADINGUP:
		return "ani-red-venus-fire-trap-headup";
	case EVenusState::HEADUP_IDLE:
		return "ani-red-venus-fire-trap-headup-idle";
	case EVenusState::HEADINGDOWN:
		return "ani-red-venus-fire-trap-headdown";
	case EVenusState::HEADDOWN_IDLE:
		return "ani-red-venus-fire-trap-headdown-idle";

	default:
		return "ani-red-venus-fire-trap-headup";
	}
	
}

void CVenus::ChangeState(EVenusState newState) {
	if (state.timeRemaining <= 0) {
		this->state.type = newState;
		if(newState == EVenusState::HEADINGDOWN || newState == EVenusState::HEADINGUP)
			this->state.timeRemaining = VENUS_TIME_DURATION_HEADING_DOWNUP;
		else {
			if(newState == EVenusState::HEADUP_IDLE)ShootBullet();
			this->state.timeRemaining = VENUS_TIME_DURATION_IDLE;
		}
			
	}
}
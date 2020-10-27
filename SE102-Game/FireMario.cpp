#include "FireMario.h"

CFireMario::CFireMario(float x, float y) : CMario(x, y) {
	type = MarioType::FIRE;

	bullets.push_back(new CFireBullet(0.0f, 0.0f, 1));
	bullets.push_back(new CFireBullet(0.0f, 0.0f, 1));
}

bool CFireMario::ShootBullet() {
	for (int i = 0; i < bullets.size(); i++) {
		// Dang aanr thi dc ban
		if (bullets.at(i)->isDisable) {
			bullets.at(i)->PrepareForShooting();
			bullets.at(i)->UpdatePos(Vector2(this->x, this->y), this->nx);
			bullets.at(i)->isDisable = false;
			return true;
		}
	}
	return false;
}

void CFireMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario::Update(dt, coObjects);
	// SKID

	
	
}

void CFireMario::Render(Vector2 finalPos)
{
	CMario::Render(finalPos);
}

void CFireMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = x + FIRE_MARIO_BBOX_WIDTH;
	bottom = y + FIRE_MARIO_BBOX_HEIGHT;
}

void CFireMario::SetState(int state)
{
	CMario::SetState(state);

	switch (state)
	{
		case MARIO_STATE_PRESS_Z:
			if(ShootBullet()) isShowingSpecialAni = GetAnimationId(MarioAction::THROW);;
			break;

	
	}

}
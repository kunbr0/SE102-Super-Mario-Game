#include "RedRaccoonMario.h"

CRedRaccoonMario::CRedRaccoonMario(float x, float y) : CMario(x, y) {
	type = MarioType::RED_RACCON;

}

void CRedRaccoonMario::Render(Vector2 finalPos)
{
	CMario::Render(finalPos);

}

void CRedRaccoonMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state.action == MarioAction::ATTACK) {
		left = x - GetBoundingBoxSize(type, state.action).x / 2 - 10;
		top = y - GetBoundingBoxSize(type, state.action).x /2 + 25;
		right = x + GetBoundingBoxSize(type, state.action).x / 2 + 10;
		bottom = y + GetBoundingBoxSize(type, state.action).y / 2;
	}
	else {
		CMario::GetBoundingBox(left, top, right, bottom);
	}
	
}

void CRedRaccoonMario::CollidedLeftRight(vector<LPCOLLISIONEVENT>* coEvents) {
	if (state.action == MarioAction::DIE) return;
	for (UINT i = 0; i < coEvents->size(); i++) {
		coEvents->at(i)->obj->BeingCollidedLeftRight(this);
	}
	CMario::CollidedLeftRight(coEvents);
}


void CRedRaccoonMario::ProcessKeyboard(SKeyboardEvent kEvent)
{
	CMario::ProcessKeyboard(kEvent);

	switch (kEvent.key)
	{
	case DIK_A:
		if (!kEvent.isHold && !kEvent.isKeyUp) ChangeAction(MarioAction::ATTACK, 450);
		break;

	case DIK_S:
		if (!kEvent.isHold && !kEvent.isKeyUp) ChangeAction(MarioAction::FLY, 350);
		if (!kEvent.isHold && !kEvent.isKeyUp) ChangeAction(MarioAction::FALL_SLIGHTLY, 200);
		break;
	}
}



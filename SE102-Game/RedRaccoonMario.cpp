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
		left = x - 20;
		top = y;
		right = x + GetBoundingBoxSize(type, state.action).x + 20;
		bottom = y + GetBoundingBoxSize(type, state.action).y;
	}
	else {
		CMario::GetBoundingBox(left, top, right, bottom);
	}
	
}

void CRedRaccoonMario::CollidedLeftRight(vector<LPCOLLISIONEVENT>* coEvents) {
	if (state.action == MarioAction::DIE) return;
	for (UINT i = 0; i < coEvents->size(); i++) {
		EActionTag tag = state.action == MarioAction::ATTACK ? EActionTag::MARIO_ATTACK : EActionTag::MARIO_DEFAULT;
		coEvents->at(i)->obj->BeingCollidedLeftRight(tag, Vector2(x, y));
	}
	CMario::CollidedLeftRight(coEvents);
}


void CRedRaccoonMario::ProcessKeyboard(SKeyboardEvent kEvent)
{
	CMario::ProcessKeyboard(kEvent);

	switch (kEvent.key)
	{
	case DIK_A:
		if (!kEvent.isHold) ChangeAction(MarioAction::ATTACK, 450);
		break;

	case DIK_S:
		if (!kEvent.isHold) ChangeAction(MarioAction::FLY, 350);
		if (!kEvent.isHold) ChangeAction(MarioAction::FALL_SLIGHTLY, 350);
		break;
	}
}



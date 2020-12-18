#include "RedRaccoonMario.h"
#define DISTANCE_OF_ATTACK_BOUNDINGBOX_FROM_CENTER		Vector2(0, 20)

CRedRaccoonMario::CRedRaccoonMario(float x, float y) : CMario(x, y) {
	type = MarioType::RED_RACCON;
	attackBoundingBox = CRaccoonAttackBoundingBox();
}

void CRedRaccoonMario::Render(Vector2 finalPos)
{
	CMario::Render(finalPos);
	//RenderBoundingBox(finalPos);
	//attackBoundingBox.Render(finalPos + DISTANCE_OF_ATTACK_BOUNDINGBOX_FROM_CENTER);
	
}

void CRedRaccoonMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state.action == MarioAction::ATTACK) attackBoundingBox.SetIsOpening(true);
	else attackBoundingBox.SetIsOpening(false);
	
	CMario::Update(dt, coObjects);
	
	attackBoundingBox.UpdatePosition(GetPosition() + DISTANCE_OF_ATTACK_BOUNDINGBOX_FROM_CENTER);
	
	attackBoundingBox.Update(dt, coObjects);
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
		if (!kEvent.isHold && !kEvent.isKeyUp) {
			if (ChangeAction(MarioAction::ATTACK, 430))
				attackBoundingBox.SetHasAttacked(false);
		}
		break;

	case DIK_S:
		if (!kEvent.isHold && !kEvent.isKeyUp) ChangeAction(MarioAction::FLY, 350);
		if (!kEvent.isHold && !kEvent.isKeyUp) ChangeAction(MarioAction::FALL_SLIGHTLY, 200);
		break;
	}
}



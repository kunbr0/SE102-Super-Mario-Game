#include "RedRaccoonMario.h"

CRedRaccoonMario::CRedRaccoonMario(float x, float y) : CMario(x, y) {
	type = MarioType::RED_RACCON;

}

void CRedRaccoonMario::Render(Vector2 finalPos)
{
	CMario::Render(finalPos);

}


void CRedRaccoonMario::ProcessKeyboard(SKeyboardEvent kEvent)
{
	CMario::ProcessKeyboard(kEvent);

	switch (kEvent.key)
	{
	case DIK_A:
		if (!kEvent.isHold) ChangeAction(MarioAction::ATTACK, 550);
		break;

	case DIK_S:
		if (!kEvent.isHold) ChangeAction(MarioAction::FLY, 350);
		if (!kEvent.isHold) ChangeAction(MarioAction::FALL_SLIGHTLY, 350);
		break;
	}
}



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
		if (!kEvent.isHold) ChangeAction(MarioAction::ATTACK);
		break;

	case DIK_S:
		if (!kEvent.isHold && powerX >= 6000) ChangeAction(MarioAction::FLY);
		break;
	}
}



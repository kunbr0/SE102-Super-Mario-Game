#include "SampleKeyEventHandler.h"

CSampleKeyHander::CSampleKeyHander(CMario* mario, CGame* game) {
	this->ObjectNeedToBeHandled = mario;
	this->game = game;
}

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		ObjectNeedToBeHandled->SetState(MARIO_STATE_JUMP);
		
		break;
	case DIK_A: // reset
		ObjectNeedToBeHandled->SetState(MARIO_STATE_IDLE);
		ObjectNeedToBeHandled->SetLevel(MARIO_LEVEL_BIG);
		ObjectNeedToBeHandled->SetPosition(50.0f, 0.0f);
		ObjectNeedToBeHandled->SetSpeed(0, 0);
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE* states)
{
	// disable control key when Mario die 
	if (ObjectNeedToBeHandled->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		ObjectNeedToBeHandled->SetState(MARIO_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		ObjectNeedToBeHandled->SetState(MARIO_STATE_WALKING_LEFT);
	else
		ObjectNeedToBeHandled->SetState(MARIO_STATE_IDLE);
}
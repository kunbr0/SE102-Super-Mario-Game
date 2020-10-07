#pragma once

#include "KeyEventHandler.h"
#include "debug.h"
#include "Mario.h"
#include "Game.h"

class CSampleKeyHander : public CKeyEventHandler
{
	CMario* ObjectNeedToBeHandled;
	CGame* game;
	CSampleKeyHander(CMario* mario, CGame* game);
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander* keyHandler;
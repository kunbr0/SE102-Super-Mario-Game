#pragma once
#include "Mario.h"
#include "RaccoonAttackBoundingBox.h"

class CRedRaccoonMario : public CMario
{
private:
	
	CRaccoonAttackBoundingBox attackBoundingBox;

public:
	CRedRaccoonMario(float x = 0.0f, float y = 0.0f);
	void Render(Vector2 finalPos);
	void Update(DWORD, vector<LPGAMEOBJECT>* = NULL) override;
	

	void ProcessKeyboard(SKeyboardEvent kEvent);
};


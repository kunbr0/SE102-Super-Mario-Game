#pragma once
#include "RectPlatform.h"

class CMovablePlatform : public CRectPlatform
{
	bool isFalling;
public:
	CMovablePlatform(float, float, float, float);
	virtual void Update(DWORD, vector<LPGAMEOBJECT>*);
	virtual void Render(Vector2 finalPos);
	virtual void BeingCollidedTop(LPGAMEOBJECT) override;
};


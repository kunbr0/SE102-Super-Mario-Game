#pragma once
#include "GameObject.h"
#include "Transform.h"

class CMiniPortal : public CGameObject
{
protected:

	Vector2 portalSize;
	Vector2 targetPosition;
	Vector2 targetSize;
	Vector2 targetCameraLeffTopLimit;
	Vector2 targetCameraRightBottomLimit;

public:
	CMiniPortal(Vector2 = Vector2(0, 0), Vector2 = Vector2(0, 0), Vector2 = Vector2(0, 0), Vector2 = Vector2(0, 0), Vector2 = Vector2(0,0), Vector2 = Vector2(0, 0));
	void Update(DWORD, vector<LPGAMEOBJECT>*) override;
	void Collided(vector<LPCOLLISIONEVENT>*) override;
	void GetBoundingBox(float&, float&, float&, float&) override;
};


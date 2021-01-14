#pragma once
#include "GameObject.h"

class CCameraLimitController : public CGameObject
{
	
public:
	CCameraLimitController(Vector2);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};

	virtual void Update(DWORD, vector<LPGAMEOBJECT>*) override;
	virtual void Render(Vector2 finalPos) {};
	
};


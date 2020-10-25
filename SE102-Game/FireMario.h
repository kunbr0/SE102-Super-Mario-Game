#pragma once
#include "Mario.h"
#include "FireBullet.h"

class CFireMario : public CMario
{
private:
	vector<CFireBullet> shootedBullets;

public:
	CFireMario(float x = 0.0f, float y = 0.0f);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render(Vector2 finalPos);

	void SetState(int state);

	void AddShootedBullet();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};


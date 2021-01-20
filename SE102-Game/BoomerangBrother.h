#pragma once
#include "Enemy.h"


class CBoomerangBrother : public CEnemy
{
	int renderNX = 1;
public:
	CBoomerangBrother(float, float);
	float GetDefaultWalkingSpeed() override { return 0.09f; }
	virtual std::string GetAnimationIdFromState() override;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	virtual void Render(Vector2) override;

	bool ShootBullet();

	virtual Vector2 GetBoundingBoxSize(EEnemyState) override;
};


#pragma once
#include "KoopasFly.h"

class CKoopasUpDown : public CKoopasFly
{
private:
	Vector2 initPos;
	int remainingTimeToChangeDirection;
	int direction;
public:
	CKoopasUpDown(float x, float y);
	std::string GetAnimationIdFromState() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
};


#pragma once
#include "Enemy.h"
#define VENUS_TIME_DURATION_PER_STATE		1500

enum class EVenusState {
	HEADUP,
	HEADDOWN
};

struct SVenusState {
	EVenusState type = EVenusState::HEADDOWN;
	int timeRemaining = 0;
};

class CVenus :  public CEnemy
{
protected: 
	Vector2 initPos;
	Vector2 size;
	SVenusState state;
public:
	CVenus(Vector2, Vector2);

	
	void Update(DWORD, vector<LPGAMEOBJECT>*) override;

	void ChangeState(EVenusState);
	std::string GetAnimationIdFromState() override;

	void GetBoundingBox(float&, float&, float&, float&) override;
};


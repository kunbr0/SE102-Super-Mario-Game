#pragma once
#include "Enemy.h"
#define VENUS_TIME_DURATION_HEADING_DOWNUP		1500
#define VENUS_TIME_DURATION_IDLE				500

enum class EVenusState {
	HEADUP_IDLE,
	HEADINGUP,
	HEADDOWN_IDLE,
	HEADINGDOWN
};

struct SVenusState {
	EVenusState type = EVenusState::HEADDOWN_IDLE;
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

	bool ShootBullet();
	
	void Update(DWORD, vector<LPGAMEOBJECT>*) override;

	void ChangeState(EVenusState);
	std::string GetAnimationIdFromState() override;

	void GetBoundingBox(float&, float&, float&, float&) override;
};


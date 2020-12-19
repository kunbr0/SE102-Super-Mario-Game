#pragma once
#include "GameObject.h"

struct SClaimBonusEffect {
	bool isActive = false;
	Vector2 totalMoveDistance = Vector2(0, -300);
	int remainingTime;
	const int totalTime = 1500;
};

class CEndSceneItem : public CGameObject
{
	const Vector2 size = Vector2(48, 48);
	SClaimBonusEffect claimBonusEffect;

public:
	CEndSceneItem(Vector2);
	void Render(Vector2 finalPos) override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};


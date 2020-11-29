#pragma once
#include "Effect.h"
class CAddingPointEffect : public CEffect
{
public:
	CAddingPointEffect(Vector2 initPos, Vector2 deltaPos) : CEffect(initPos, deltaPos) {};
	std::string GetAnimationId() override;
	void Update(DWORD) override;
	void OnFinish() override;
};


#pragma once
#include "QuestionBlock.h"


class CGoldenBrick : public CQuestionBlock
{
private:
	Vector2 virtualCenter;

public:
	CGoldenBrick(Vector2);


	virtual void Render(Vector2 finalPos);
	Vector2 GetBoundingBoxSizeFromState();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	void OnHadCollided(LPGAMEOBJECT) override;
	void BeingCollidedLeftRight(LPGAMEOBJECT) override;

	virtual void Update(DWORD, vector<LPGAMEOBJECT>*) override;
	virtual void ChangeState(EBlockState, DWORD = 0);
	virtual void OpenBox();


	std::string GetAnimationIdFromState();


};


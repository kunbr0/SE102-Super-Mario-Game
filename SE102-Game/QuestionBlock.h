#pragma once
#include "RectCollision.h"
#define QUESTION_BOX_SIZE				48
#define QUESTION_BOX_ANIMATION_DEFAULT_ID		"ani-question-block"
#define QUESTION_BOX_ANIMATION_OPENED_ID		"ani-empty-block"


enum class EBlockState {
	DEFAULT,
	OPENED,

};

class CQuestionBlock : public CRectCollision
{
private:
	EBlockState state;
public:
	CQuestionBlock(Vector2 initPos);
	void Render(Vector2 finalPos);
	void ChangeState(EBlockState newState);
	std::string GetAnimationIdFromState();

	void BeingCollidedBottom(LPGAMEOBJECT) override;
};




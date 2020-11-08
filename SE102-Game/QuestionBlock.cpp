#include "QuestionBlock.h"

CQuestionBlock::CQuestionBlock(Vector2 initPos) :
	CRectCollision(initPos.x, initPos.y, QUESTION_BOX_SIZE, QUESTION_BOX_SIZE){
	state = EBlockState::DEFAULT;
}

void CQuestionBlock::BeingCollidedBottom(ETag tag, Vector2 pos) {
	ChangeState(EBlockState::OPENED);
}

void CQuestionBlock::Render(Vector2 finalPos) {
	CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(Vector2(finalPos));
}

std::string CQuestionBlock::GetAnimationIdFromState() {
	switch (state)
	{
	case EBlockState::DEFAULT:
		return QUESTION_BOX_ANIMATION_DEFAULT_ID;
	case EBlockState::OPENED:
		return QUESTION_BOX_ANIMATION_OPENED_ID;
	default:
		break;
	}
}

void CQuestionBlock::ChangeState(EBlockState newState) {
	switch (state)
	{
	case EBlockState::DEFAULT:
		state = newState;
		break;
	case EBlockState::OPENED:
		if (state == EBlockState::DEFAULT)
			state = newState;
		break;
	default:
		break;
	}
}
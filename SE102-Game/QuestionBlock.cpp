#include "QuestionBlock.h"



CQuestionBlock::CQuestionBlock(Vector2 initPos) :
	CRectCollision(initPos.x, initPos.y, QUESTION_BOX_SIZE, QUESTION_BOX_SIZE){
	state = EBlockState::DEFAULT;
	deltaRender = Vector2(0, -4);
	startOpen = 0;
}

void CQuestionBlock::BeingCollidedBottom(EActionTag tag, Vector2 pos) {
	ChangeState(EBlockState::OPENED);
	
}

void CQuestionBlock::Render(Vector2 finalPos) {
	if (GetTickCount() - startOpen < QUESTION_BOX_OPEN_TOTAL_TIME) {
		int totalSteps = QUESTION_BOX_OPEN_TOTAL_TIME / QUESTION_BOX_OPEN_EACH_TIME;
		int step = (GetTickCount() - startOpen) / QUESTION_BOX_OPEN_EACH_TIME;
		if (step > totalSteps / 2) step = totalSteps - step;
		CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(
			Vector2(finalPos.x + deltaRender.x* step, finalPos.y + deltaRender.y * step));
	}
	else
		CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(Vector2(finalPos));
	//RenderBoundingBox(finalPos);
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
	switch (newState)
	{
	case EBlockState::DEFAULT:
		state = newState;
		break;
	case EBlockState::OPENED:
		if (state == EBlockState::DEFAULT) {
			startOpen = GetTickCount();
			state = newState;
		}
			
		break;
	default:
		break;
	}
}
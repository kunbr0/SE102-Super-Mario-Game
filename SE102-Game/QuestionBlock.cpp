#include "QuestionBlock.h"
#include "PlayScene.h"
#define DELTA_POSITION_STEP_OPENING			7

CQuestionBlock::CQuestionBlock(Vector2 initPos) :
	CRectCollision(initPos.x, initPos.y, QUESTION_BOX_SIZE, QUESTION_BOX_SIZE){
	state.type = EBlockState::DEFAULT;
	deltaRender = Vector2(0, 0);

}

void CQuestionBlock::BeingCollidedBottom(LPGAMEOBJECT) {
	ChangeState(EBlockState::OPENING, 200);
}

void CQuestionBlock::Render(Vector2 finalPos) {
	CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(Vector2(finalPos.x + deltaRender.x, finalPos.y+deltaRender.y));
	
	RenderBoundingBox(finalPos);
}



void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
	if (state.type == EBlockState::OPENING) {
		ChangeState(EBlockState::OPENED);
	}
}




std::string CQuestionBlock::GetAnimationIdFromState() {
	switch (state.type)
	{
	case EBlockState::DEFAULT:
		return QUESTION_BOX_ANIMATION_DEFAULT_ID;

	case EBlockState::OPENING:
	case EBlockState::OPENED:
		return QUESTION_BOX_ANIMATION_OPENED_ID;
	default:
		break;
	}
}




void CQuestionBlock::OpenBox() {
	
	
}




void CQuestionBlock::SetState(EBlockState newState, DWORD newTimeState) {
	state.type = newState;
}


void CQuestionBlock::ChangeState(EBlockState newState, DWORD newTimeState) {
	
	switch (newState)
	{
	case EBlockState::DEFAULT:
		SetState(newState, newTimeState);
		break;
	case EBlockState::OPENING:
		if (state.type == EBlockState::DEFAULT) {
			if (dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())) {
				((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushTempObjects(this);
				isTemp = true;
			}
			OpenBox();
			SetState(newState, newTimeState);
		}
			
		break;

	case EBlockState::OPENED:
		SetState(newState, newTimeState);
		CGame::GetInstance()->GetCurrentScene()->GetCamera()->AdjustScore(100);
		break;

	default:
		break;
	}
}
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
	if (extra_animations.size() > 0) RenderExtraAnimations();
	RenderBoundingBox(finalPos);
}

void CQuestionBlock::RenderExtraAnimations() {
	for (MotionableAnimation mA : extra_animations) {
		DWORD curTime = GetTickCount64();
		if (curTime > mA.timeBegin && curTime < mA.timeBegin + mA.timeAni) {
			float ratio = (curTime * 1.0f - mA.timeBegin * 1.0f) / (mA.timeAni * 1.0f);
			Vector2 finalPos = CGame::GetInstance()->GetCurrentScene()->GetCamera()->ConvertPosition(mA.initialPosition);
			
			CAnimations::GetInstance()->Get(mA.animationId)->Render(
				Vector2((int)(finalPos.x + mA.deltaPosition.x * ratio), (int)(finalPos.y + mA.deltaPosition.y * ratio))
			);
		}
	}
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
	if (state.type == EBlockState::OPENING) {
		ChangeState(EBlockState::OPENED);
		OpeningBox();
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

MotionableAnimation CQuestionBlock::CreateMotionableAnimation(std::string id, Vector2 initPos, Vector2 deltaPos, int begin, int timeAni) {
	MotionableAnimation a;
	a.animationId = id;
	a.initialPosition = initPos;
	a.deltaPosition = deltaPos;
	a.timeBegin = begin;
	a.timeAni = timeAni;
	return a;
}

void CQuestionBlock::CreateBoundingCoinAnimation(Vector2 initPos) {
	extra_animations.push_back(CreateMotionableAnimation("ani-coin", initPos + Vector2(0, -48), Vector2(0, -200), GetTickCount64(), 1000));
	extra_animations.push_back(CreateMotionableAnimation("ani-coin", initPos + Vector2(0, -248), Vector2(0, 200), GetTickCount64() + 1000, 1000));

}

void CQuestionBlock::OpeningBox() {
	if (state.type != EBlockState::OPENING) return;
	int currentTimeInState = GetTickCount64() - state.beginState;
	float step = ((currentTimeInState*10 / (state.timeState+1))) + 1;
	DebugOut(ToWSTR(std::to_string(step) + "\n").c_str());
	if (step <= 5) deltaRender.y = -DELTA_POSITION_STEP_OPENING * step;
	else deltaRender.y = -DELTA_POSITION_STEP_OPENING * (10 - step);

}



void CQuestionBlock::SetState(EBlockState newState, DWORD newTimeState) {
	state.type = newState;
	state.beginState = GetTickCount64();
	state.timeState = newTimeState;
}


void CQuestionBlock::ChangeState(EBlockState newState, DWORD newTimeState) {
	if (GetTickCount64() < state.timeState + state.beginState) return;
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
			CreateBoundingCoinAnimation(GetPosition() + Vector2(3,0));
			SetState(newState, newTimeState);
		}
			
		break;

	case EBlockState::OPENED:
		SetState(newState, newTimeState);
		break;

	default:
		break;
	}
}
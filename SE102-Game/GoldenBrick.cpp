#include "GoldenBrick.h"
#include "PlayScene.h"
#include "BoundingCoinEffect.h"
#include "RaccoonAttackBoundingBox.h"

#define DELTA_POSITION_STEP_OPENING			7

CGoldenBrick::CGoldenBrick(Vector2 initPos) : CQuestionBlock(initPos) {
	virtualCenter = Vector2(0,0);
	deltaRender = Vector2(0, 0);
}


void CGoldenBrick::Render(Vector2 finalPos) {
	if (state.type == EBlockState::DEFAULT) {
		CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(Vector2(finalPos.x, finalPos.y));
	}
	else if (state.type == EBlockState::OPENING) {
		CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(Vector2(deltaRender.x*1, deltaRender.y*1) + finalPos + virtualCenter, Vector2(0.6f,0.6f));
		CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(Vector2(deltaRender.x*-1, deltaRender.y*1) + finalPos + virtualCenter, Vector2(0.6f, 0.6f));
		CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(Vector2(deltaRender.x*1, deltaRender.y*-1) + finalPos + virtualCenter, Vector2(0.6f, 0.6f));
		CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(Vector2(deltaRender.x*-1, deltaRender.y*-1) + finalPos + virtualCenter, Vector2(0.6f, 0.6f));
	}
	//RenderBoundingBox(finalPos);
}



void CGoldenBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state.type == EBlockState::OPENING) {
		deltaRenderSpeed.y += 0.1f;
		virtualCenter.y += deltaRenderSpeed.y;
		
		deltaRender += Vector2(0.6f, 0.6f);
		
		ChangeState(EBlockState::OPENED);
	}

}

void CGoldenBrick::OpenBox() {
	deltaRenderSpeed.y = -5.2;
	//((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEffects(new CBoundingCoinEffect(Vector2(x, y - 48 * 1.5), Vector2(0, -0.50)));
}

Vector2 CGoldenBrick::GetBoundingBoxSizeFromState() {
	switch (state.type)
	{
	case EBlockState::DEFAULT:
		return Vector2(48,48);

	case EBlockState::OPENING:
	case EBlockState::OPENED:
		return Vector2(0, 0);
	default:
		return Vector2(0, 0);
	}
}

void CGoldenBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	Vector2 size = GetBoundingBoxSizeFromState();
	left = x - size.x / 2;
	top = y - size.y / 2;
	right = x + size.x / 2;
	bottom = y + size.y / 2;
}

std::string CGoldenBrick::GetAnimationIdFromState() {
	switch (state.type)
	{
	case EBlockState::DEFAULT:
		return "ani-brick";

	case EBlockState::OPENING:
	case EBlockState::OPENED:
		return "ani-brick-debris";
	default:
		break;
	}
}

void CGoldenBrick::OnHadCollided(LPGAMEOBJECT obj) {
	this->BeingCollidedLeftRight(obj);
};

void CGoldenBrick::BeingCollidedLeftRight(LPGAMEOBJECT obj) {
	if (dynamic_cast<CRaccoonAttackBoundingBox*>(obj)) {
		ChangeState(EBlockState::OPENING, 3000);
	}
	
}








void CGoldenBrick::ChangeState(EBlockState newState, DWORD newTimeState) {
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
			OpenBox();
			SetState(newState, newTimeState);
		}

		break;

	case EBlockState::OPENED:
		SetState(newState, newTimeState);
		isTemp = false;
		break;

	default:
		break;
	}
}
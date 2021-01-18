#include "GoldenBrick.h"
#include "PlayScene.h"
#include "BoundingCoinEffect.h"
#include "RaccoonAttackBoundingBox.h"
#include "AddingPointEffect.h"
#include "Mario.h"
#include "PlayScene.h"

#define DELTA_POSITION_STEP_OPENING			7

CGoldenBrick::CGoldenBrick(Vector2 initPos, string hiddenItem) : CQuestionBlock(initPos) {
	virtualCenter = Vector2(0, 0);
	deltaRender = Vector2(0, 0);
	if (hiddenItem == "P") this->hiddenItem = EHiddenItem::P;
}


void CGoldenBrick::Render(Vector2 finalPos) {
	if (state.type == EBlockState::DEFAULT) {
		CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(finalPos);
	}
	else if (state.type == EBlockState::OPENING) {
		if (hiddenItem == EHiddenItem::NONE) {
			CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(Vector2(deltaRender.x * 1, deltaRender.y * 1) + finalPos + virtualCenter, Vector2(0.6f, 0.6f));
			CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(Vector2(deltaRender.x * -1, deltaRender.y * 1) + finalPos + virtualCenter, Vector2(0.6f, 0.6f));
			CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(Vector2(deltaRender.x * 1, deltaRender.y * -1) + finalPos + virtualCenter, Vector2(0.6f, 0.6f));
			CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(Vector2(deltaRender.x * -1, deltaRender.y * -1) + finalPos + virtualCenter, Vector2(0.6f, 0.6f));
		}
		else if (hiddenItem == EHiddenItem::P) {
			CSprites::GetInstance()->Get("spr-empty-block-0")->DrawWithScaling(finalPos);
			CAnimations::GetInstance()->Get("ani-p")->Render(finalPos - Vector2(0,48));
		}
	}
	else if (state.type == EBlockState::OPENED) {
		if (hiddenItem == EHiddenItem::P) {
			CSprites::GetInstance()->Get("spr-empty-block-0")->DrawWithScaling(finalPos);
			CSprites::GetInstance()->Get("spr-p-2")->DrawWithScaling(finalPos - Vector2(0,36));
		}
	}
	else if (state.type == EBlockState::SHOW_HIDDEN_COIN) {
		CSprites::GetInstance()->Get("spr-coin-3")->DrawWithScaling(finalPos);
	}
	RenderBoundingBox(finalPos);
}

void CGoldenBrick::BeingCollidedTop(LPGAMEOBJECT obj) {
	CGameObject::BeingCollidedTop(obj);
	if (dynamic_cast<CMario*>(obj)) {
		if (state.type == EBlockState::OPENING) {
			ChangeState(EBlockState::OPENED);
			((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->ChangeGoldenBricksInCameraToShowCoinState();
			((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->BeginVibratingCamera(400);
		}
	}
}

void CGoldenBrick::BeingCollided(LPGAMEOBJECT obj) {
	if (isDisable) return;
	if (state.type == EBlockState::SHOW_HIDDEN_COIN) {
		if (dynamic_cast<CMario*>(obj)) {
			isDisable = true;
			ChangeState(EBlockState::OPENED);
			((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEffects(new CAddingPointEffect(Vector2(x, y), Vector2(0, -0.13)));

		}
	}
	else if (dynamic_cast<CRaccoonAttackBoundingBox*>(obj) && state.type == EBlockState::DEFAULT) {
		if (hiddenItem == EHiddenItem::NONE) ChangeState(EBlockState::OPENING, 3000);
		else if (hiddenItem == EHiddenItem::P) ChangeState(EBlockState::OPENING);
	}
}

void CGoldenBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state.type == EBlockState::OPENING) {
		deltaRenderSpeed.y += 0.35f;
		virtualCenter.y += deltaRenderSpeed.y;

		deltaRender += Vector2(0.9f, 0.9f);

		ChangeState(EBlockState::OPENED);
	}

}

void CGoldenBrick::OpenBox() {
	deltaRenderSpeed.y = -10.2;
	
}

Vector2 CGoldenBrick::GetBoundingBoxSizeFromState() {
	if (hiddenItem == EHiddenItem::NONE) {
		switch (state.type)
		{
		case EBlockState::DEFAULT:
			return Vector2(48, 48);

		case EBlockState::OPENING:
		case EBlockState::OPENED:
			return Vector2(0, 0);
		case EBlockState::SHOW_HIDDEN_COIN:
			return Vector2(48, 48);
		default:
			return Vector2(0, 0);
		}
	}
	else if (hiddenItem == EHiddenItem::P) {
		switch (state.type)
		{
		case EBlockState::DEFAULT:
			return Vector2(48, 48);

		case EBlockState::OPENING:
		case EBlockState::OPENED:
			return Vector2(48, 48);

		default:
			return Vector2(0, 0);
		}
	}
	
}

void CGoldenBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	Vector2 size = GetBoundingBoxSizeFromState();
	
	left = x - size.x / 2;
	top = y - size.y / 2;
	right = x + size.x / 2;
	bottom = y + size.y / 2;

	if (hiddenItem == EHiddenItem::P && state.type == EBlockState::OPENING) {
		top = y - size.y / 2 - 48;
	}
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
	BeingCollided(obj);
};



void CGoldenBrick::ChangeState(EBlockState newState, DWORD newTimeState) {
	if (GetTickCount64() < state.timeState + state.beginState) return;
	switch (newState)
	{
	case EBlockState::DEFAULT:
		SetState(newState, newTimeState);
		break;
	case EBlockState::OPENING:
		if (state.type == EBlockState::DEFAULT) {
			if (hiddenItem == EHiddenItem::NONE) {
				if (dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())) {
					((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushTempObjects(this);
					isTemp = true;
				}
				OpenBox();
			}
			else if (hiddenItem == EHiddenItem::P) {

			}
			
			SetState(newState, newTimeState);
		}

		break;

	case EBlockState::OPENED:
		SetState(newState, newTimeState);
		isTemp = false;
		break;
	case EBlockState::SHOW_HIDDEN_COIN:
		if (hiddenItem != EHiddenItem::P) {
			SetState(newState, newTimeState);
			allowOthersGoThrough = true;
		}
		break;
	default:
		break;
	}
}
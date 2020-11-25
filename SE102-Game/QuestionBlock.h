#pragma once
#include "RectCollision.h"
#define QUESTION_BOX_SIZE				48
#define QUESTION_BOX_ANIMATION_DEFAULT_ID		"ani-question-block"
#define QUESTION_BOX_ANIMATION_OPENED_ID		"ani-empty-block"
#define QUESTION_BOX_OPEN_TOTAL_TIME			200
#define QUESTION_BOX_OPEN_EACH_TIME				20

enum class EBlockState {
	DEFAULT,
	OPENING,
	OPENED
};

struct MotionableAnimation {
	std::string animationId;
	Vector2 initialPosition;
	Vector2 deltaPosition;
	DWORD timeBegin;
	DWORD timeAni;
};

struct MotionableAnimations {
	
};

struct SBlockState {
	EBlockState type = EBlockState::DEFAULT;
	DWORD beginState = 0;
	DWORD timeState = 0;
};

class CQuestionBlock : public CRectCollision
{
private:
	SBlockState state;
	vector<MotionableAnimation> extra_animations;

	Vector2 deltaRender;

public:
	
	

	CQuestionBlock(Vector2 initPos);
	void Render(Vector2 finalPos);
	void RenderExtraAnimations();
	void Update(DWORD, vector<LPGAMEOBJECT>* ) override;
	void ChangeState(EBlockState, DWORD = 0);
	void SetState(EBlockState, DWORD = 0);
	void OpeningBox();
	
	std::string GetAnimationIdFromState();
	MotionableAnimation CreateMotionableAnimation(std::string, Vector2, Vector2, int, int);
	void CreateBoundingCoinAnimation(Vector2);
	void BeingCollidedBottom(LPGAMEOBJECT) override;
};




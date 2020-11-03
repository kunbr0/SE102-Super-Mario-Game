#pragma once
#include "RectCollision.h"
#define QUESTION_BOX_SIZE				48
#define QUESTION_BOX_ANIMATION_ID		"ani-question-block"

class CQuestionBox : public CRectCollision
{
private:
public:
	CQuestionBox(Vector2 initPos);
	void Render(Vector2 finalPos);
};




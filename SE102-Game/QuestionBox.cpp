#include "QuestionBox.h"

CQuestionBox::CQuestionBox(Vector2 initPos) : 
	CRectCollision(initPos.x, initPos.y, QUESTION_BOX_SIZE, QUESTION_BOX_SIZE)
{

}


void CQuestionBox::Render(Vector2 finalPos) {
	CAnimations::GetInstance()->Get(QUESTION_BOX_ANIMATION_ID)->Render(Vector2(finalPos));

}
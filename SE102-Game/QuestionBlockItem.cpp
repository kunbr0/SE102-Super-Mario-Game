#include "QuestionBlockItem.h"
#include "Leaf.h"
#include "PlayScene.h"

CQuestionBlockItem::CQuestionBlockItem(Vector2 initPos) : CQuestionBlock(initPos) {

}

void CQuestionBlockItem::OpenBox() {
	((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushDynamicObjects(new CLeaf(GetPosition()));
	ChangeState(EBlockState::OPENING, 200);

}
#include "QuestionBlockItem.h"
#include "Leaf.h"
#include "PlayScene.h"

CQuestionBlockItem::CQuestionBlockItem(Vector2 initPos) : CQuestionBlock(initPos) {

}

void CQuestionBlockItem::OpenBox() {
	((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushTempObjects(new CLeaf(GetPosition()));
	

}
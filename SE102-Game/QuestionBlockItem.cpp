#include "QuestionBlockItem.h"
#include "Leaf.h"
#include "PlayScene.h"

CQuestionBlockItem::CQuestionBlockItem(Vector2 initPos) : CQuestionBlock(initPos) {

}

void CQuestionBlockItem::OpenBox() {
	LPGAMEOBJECT obj = new CLeaf(GetPosition() - Vector2(0,48));
	obj->AddPriority(EPriorityFlag::TEMP_OBJECT);
	((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetCamera()->GetMap()->GetGrid()->AddObjectToGrid(obj);
	//((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushTempObjects();
	

}
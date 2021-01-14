#include "Camera.h"
#include "Game.h"
#include "UIDrawer.h"
#include <iomanip>
#include "Mario.h"
#include "Mathf.h"

#define DELTA_CHANGE_CAM_BASE_POSITION			5

#define DetailsBoardHeight		200
#define YGROUND					710


CCamera::CCamera() {
	this->camPosition = Vector2(0, 0);
	this->camSize = Vector2(CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight());
	this->positionController = NULL;
	mapData.timeRemaining = 300 * 1000;
}
CCamera::~CCamera() {

}

void CCamera::InitPositionController(CGameObject* player) {
	this->positionController = player;
}

void CCamera::InitMario(CGameObject* player) {
	this->mario = player;
}

void CCamera::LoadMap(std::string mapFilePath, LPGAMEOBJECT* camLimitCon, vector<LPGAMEOBJECT>* staticObjects, vector<LPGAMEOBJECT>* dynamicObjects, vector<LPGAMEOBJECT>* dynamicObjectsBehindMap, vector<LPGAMEOBJECT>* tempObjects) {
	mMap = CGameMap().FromTMX(mapFilePath, camLimitCon, staticObjects, dynamicObjects, dynamicObjectsBehindMap, tempObjects);
	mMap->GetMapSize(mapSize);
	
}

void CCamera::LoadMap(std::string mapFilePath, vector<LPGAMEOBJECT>* staticObjects, std::unordered_map<std::string, CSelectionNode*>* selectionNodes) {
	mMap = CGameMap().FromTMX(mapFilePath, staticObjects, selectionNodes);
	mMap->GetMapSize(mapSize);

}

void CCamera::LoadMap(std::string mapFilePath, vector<LPGAMEOBJECT>* staticObjects) {
	mMap = CGameMap().FromTMX(mapFilePath, staticObjects);
	mMap->GetMapSize(mapSize);

}


Vector2 CCamera::GetCamPosition() {
	return camPosition;
}

void CCamera::SetMapData(int newTimeRemaning, int newScore, int playerLife) {
	if (newTimeRemaning != -1) mapData.timeRemaining = newTimeRemaning;
	if (newScore != -1) mapData.score = newScore;
	if (playerLife != -1) mapData.playerLife = playerLife;
}

void CCamera::AdjustTimeRemaining(int deltaTime) {
	mapData.timeRemaining -= deltaTime;
	if (mapData.timeRemaining < 0) mapData.timeRemaining = 0;
}

   void CCamera::AdjustScore(int deltaScore) {
	mapData.score += deltaScore;
	if (mapData.score < 0) mapData.score = 0;
}

Vector2 CCamera::ConvertPosition(Vector2 pos) {
	
	return Vector2(pos.x - camPosition.x, pos.y - camPosition.y);
}

bool CCamera::IsInCamera(Vector2 realPos, int outsideCam) {
	Vector2 finalPos = ConvertPosition((realPos));
	if (finalPos.x > 0 - outsideCam && finalPos.x < camSize.x + outsideCam 
		&& finalPos.y > 0 - outsideCam && finalPos.y < camSize.y + outsideCam)
		return true;
	return false;
}

void CCamera::ChangeCamArea(Vector2 ar1, Vector2 ar2) { 
	LeftTopLimit = ar1;
	RightBottomLimit = ar2; 
}

void CCamera::ChangeCamPosition(Vector2 newPos) {
	Vector2 newResult;
	newResult.x = newPos.x - camSize.x / 2;
	newResult.y = newPos.y - camSize.y / 2;

	
	SetCamPosition(newResult);
}


void CCamera::SetCamPosition(Vector2 pos) {
	

	if (pos.x < LeftTopLimit.x)
		pos.x = LeftTopLimit.x; // overflow left side
	if (pos.x + camSize.x > RightBottomLimit.x)
		pos.x = (int)(RightBottomLimit.x - camSize.x); // overflow right side
	
	if (pos.y < LeftTopLimit.y)
		pos.y = LeftTopLimit.y;
	
	int BottomOffSet = 0;

	if (dynamic_cast<CMario*>(mario)) {
		MarioAction marioAction = ((CMario*)(mario))->GetAction();
		bool isFlying = marioAction == MarioAction::FLY;
		bool isFalling = marioAction == MarioAction::FALL || marioAction == MarioAction::FALL_SLIGHTLY;
		if (isFlying) isLocked = false;
		if (!isLocked && marioAction == MarioAction::IDLE) isLocked = true;
	}
	


	if (isLocked) BottomOffSet = 500;

	

	if (pos.y + BottomOffSet > RightBottomLimit.y - camSize.y) {
		pos.y = RightBottomLimit.y - camSize.y;
	}
	
	camPosition = pos;
	if (mario != NULL) {
		if (mario->x < camPosition.x + 50) { 
			mario->x = camPosition.x + 50; 
			((CMario*)(mario))->ChangeAction(MarioAction::WALK);
		}
		else if (mario->x > camPosition.x + camSize.x - 50) {
			mario->x = camPosition.x + camSize.x - 50;
			((CMario*)(mario))->ChangeAction(MarioAction::WALK);
		}
	}
		
}


void CCamera::UpdateCamPosition() {
	float xMario, yMario;
	positionController->GetPosition(xMario, yMario);

	ChangeCamPosition(positionController->GetPosition());

	/*if (abs(CamBasePosition.y - yMario) < 300) {
		SetCamPosition(Vector2((int)(xMario - camSize.x / 2), (int)(CamBasePosition.y - camSize.y / 2) - 130));
	}
	else {
		SetCamPosition(Vector2((int)(xMario - camSize.x / 2), (int)(yMario - camSize.y / 2)));
	}*/
	//SetCamPosition(Vector2((int)(xMario - camSize.x / 2), (int)(CamBasePosition.y - camSize.y / 2) - 130));

	//SetCamPosition(Vector2((int)(xMario - camSize.x / 2), (int)(yMario - camSize.y / 2)));
	
}

void CCamera::Update(DWORD dt) {
	
	//CGame* game = CGame::GetInstance();
	UpdateCamPosition();
	mMap->UpdateCamPosition(camPosition);
	AdjustTimeRemaining(dt);
	
	
}

void CCamera::RenderDetailBoard() {

	LPDIRECT3DTEXTURE9 black = CTextures::GetInstance()->Get("black");
	RECT rect; rect.left = 0; rect.top = 0; rect.right = camSize.x; rect.bottom = DetailsBoardHeight;
	CGame::GetInstance()->DrawWithScaling(Vector2(camSize.x / 2, camSize.y - DetailsBoardHeight / 2 + 48), Vector2(0, 0), black, rect, 255);

	LPSPRITE hud = CSprites::GetInstance()->Get("spr-hud-0");
	LPSPRITE Mchar = CSprites::GetInstance()->Get("spr-m-tag-0");
	LPSPRITE Onechar = CSprites::GetInstance()->Get("spr-font-1");

	int currentLv = 1;
	if(mario != nullptr)
		currentLv = (int)((CMario*)mario)->GetType();

	LPSPRITE Fourchar = CSprites::GetInstance()->Get("spr-font-" + std::to_string(currentLv+1));
	Vector2 beginPos = Vector2(240, camSize.y + hud->getSize().y - DetailsBoardHeight + 20);

	

	hud->DrawWithScaling(beginPos);
	Mchar->DrawWithScaling(Vector2(beginPos.x-190, beginPos.y+14));
	Onechar->DrawWithScaling(Vector2(beginPos.x - 100, beginPos.y - 10));
	Fourchar->DrawWithScaling(Vector2(beginPos.x - 100, beginPos.y + 15));
	
	Vector2 beginScorePos = Vector2(beginPos.x - 65, beginPos.y + 15);
	int distance = 26;
	/*for (int i = 0; i < 7; i++) {
		LPSPRITE Zerochar = CSprites::GetInstance()->Get("spr-font-0");
		Zerochar->DrawWithScaling(Vector2(beginScorePos.x + i* distance, beginScorePos.y));

	}*/

	CUIDrawer::GetInstance()->DrawFixedLengthNumber(to_string(mapData.score), beginScorePos, '0', 7);
	if (mario == NULL) return;
	for (int i = 0; i < 7; i++) {
		LPSPRITE Zerochar;
		if (i < mario->powerX/1000) {
			Zerochar = CSprites::GetInstance()->Get("spr-triangle-icon-0");
		}
		else {
			Zerochar = CSprites::GetInstance()->Get("spr-triangle-icon-1");
		}
		
		if (i == 7 - 1) {
			Zerochar = CSprites::GetInstance()->Get("spr-p-icon-1");
			Zerochar->DrawWithScaling(Vector2(beginScorePos.x + i * distance + 15, beginScorePos.y - 25));
		}
		else
			Zerochar->DrawWithScaling(Vector2(beginScorePos.x + i * distance, beginScorePos.y - 25));

	}
	CUIDrawer::GetInstance()->DrawFixedLengthNumber(to_string(mapData.timeRemaining / 1000), beginScorePos + Vector2(221,0), '0', 3);
	
	vector<int> cards = CGame::GetInstance()->GetCurrentScene()->GetCards();
	for (int i = 0; i < 3; i++) {
		if(i < cards.size())
			CSprites::GetInstance()->Get("spr-card-" + std::to_string(cards[i]))->DrawWithScaling(Vector2(beginScorePos.x + 370 + i * 70, beginScorePos.y - 18));
		else
		CSprites::GetInstance()->Get("spr-empty-card-0")->DrawWithScaling(Vector2(beginScorePos.x + 370 + i*70, beginScorePos.y - 18));
	}



}

void CCamera::Render() {
	//mMap->Render(DetailsBoardHeight+150);
	mMap->Render();
	if(mario != nullptr)
		if (((CMario*)mario)->GetFinishStep() >= 2) RenderFinishPlayScene();
}

void CCamera::RenderPausing() {
	LPDIRECT3DTEXTURE9 black = CTextures::GetInstance()->Get("black");
	RECT rect; rect.left = 0; rect.top = 0; rect.right = camSize.x; rect.bottom = camSize.y;
	CGame::GetInstance()->DrawWithScaling(Vector2(camSize.x / 2, camSize.y / 2), Vector2(0, 0), black, rect, 100);
	CUIDrawer::GetInstance()->Draw("PAUSE", Vector2(camSize.x / 2 - 60, camSize.y / 2 - 10));
}

void CCamera::RenderFinishPlayScene() {
	
	CUIDrawer::GetInstance()->Draw("COURSE CLEAR", Vector2(camSize.x / 2 - 130, camSize.y / 2 - 250));
	CUIDrawer::GetInstance()->Draw("YOU GOT A CARD", Vector2(camSize.x / 2 - 190, camSize.y / 2 - 190));
	
	int cardId = CGame::GetInstance()->GetCurrentScene()->GetLastCard();
	if (cardId != -1) {
		CSprites::GetInstance()->Get("spr-card-" + std::to_string(cardId))->DrawWithScaling(Vector2(camSize.x / 2 + 210, camSize.y / 2 - 190));
	}
	
}
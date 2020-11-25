#include "Camera.h"
#include "Game.h"

#define DetailsBoardHeight		200
#define YGROUND					710


CCamera::CCamera() {
	this->camPosition = Vector2(0, 0);
	this->camSize = Vector2(CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight());
	this->positionController = NULL;
}
CCamera::~CCamera() {

}

void CCamera::InitPositionController(CGameObject* player) {
	this->positionController = player;
}

void CCamera::LoadMap(std::string mapFilePath, vector<LPGAMEOBJECT>* objects) {
	mMap = CGameMap().FromTMX(mapFilePath, objects);
	mMap->GetMapSize(mapSize);
	mapSize;
}

Vector2 CCamera::GetCamPosition() {
	return camPosition;
}

void CCamera::SetCamPosition(Vector2 pos) {
	if (pos.x < 0) pos.x = 0; // overflow left side
	if (pos.x + camSize.x > mapSize.x) pos.x = (int)(mapSize.x - camSize.x); // overflow right side
	if (pos.y < 0) pos.y = 0;
	camPosition = pos;
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

void CCamera::UpdateCamPosition() {
	float left, top, right, bottom;
	positionController->GetBoundingBox(left, top, right, bottom);

	SetCamPosition(Vector2((int)(left + right - camSize.x) / 2, YGROUND));
}

void CCamera::Update(DWORD dt) {
	
	//CGame* game = CGame::GetInstance();
	UpdateCamPosition();
	mMap->UpdateCamPosition(camPosition);
	
	
	
}

void CCamera::RenderDetailBoard() {
	//LPSPRITE hud = CSprites::GetInstance()->Get("spr-hud-0");
	//LPSPRITE Mchar = CSprites::GetInstance()->Get("spr-m-tag-0");
	//LPSPRITE Onechar = CSprites::GetInstance()->Get("spr-font-1");
	//LPSPRITE Fourchar = CSprites::GetInstance()->Get("spr-font-4");
	//Vector2 beginPos = Vector2(0, camSize.y + hud->getSize().y - DetailsBoardHeight - 20);
	//hud->Draw(beginPos);
	//Mchar->Draw(Vector2(beginPos.x+12, beginPos.y+45));
	//Onechar->Draw(Vector2(beginPos.x + 115, beginPos.y + 25));
	//Fourchar->Draw(Vector2(beginPos.x + 115, beginPos.y + 50));
	//// Point
	//Vector2 beginScorePos = Vector2(beginPos.x + 145, beginPos.y + 50);
	//int distance = 26;
	//for (int i = 0; i < 7; i++) {
	//	LPSPRITE Zerochar = CSprites::GetInstance()->Get("spr-font-0");
	//	Zerochar->Draw(Vector2(beginScorePos.x + i* distance, beginScorePos.y));

	//}
	//for (int i = 0; i < 7; i++) {
	//	LPSPRITE Zerochar;
	//	if (i < positionController->powerX/1000) {
	//		Zerochar = CSprites::GetInstance()->Get("spr-triangle-icon-0");
	//	}
	//	else {
	//		Zerochar = CSprites::GetInstance()->Get("spr-triangle-icon-1");
	//	}
	//	
	//	if (i == 7 - 1) {
	//		Zerochar = CSprites::GetInstance()->Get("spr-p-icon-1");
	//	}
	//	Zerochar->Draw(Vector2(beginScorePos.x + i * distance, beginScorePos.y - 25));

	//}
	
}

void CCamera::Render() {
	mMap->Render(DetailsBoardHeight);
	RenderDetailBoard();
}
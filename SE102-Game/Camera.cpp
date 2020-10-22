#include "Camera.h"
#include "Game.h"



CCamera::CCamera() {
	this->camPosition = Vector2(0, 0);
	this->camSize = Vector2(CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight());
	//mMap = NULL;
}
CCamera::~CCamera() {

}

void CCamera::InitPositionController(CGameObject* player) {
	this->positionController = player;
}

void CCamera::LoadMap(std::string mapFilePath, vector<LPGAMEOBJECT>* objects) {
	mMap = CGameMap().FromTMX(mapFilePath, objects);
}

Vector2 CCamera::GetCamPosition() {
	return camPosition;
}

void CCamera::SetCamPosition(Vector2 pos) {
	if (pos.x < 0) pos.x = 0;
	if (pos.y < 0) pos.y = 0;
	camPosition = pos;
}

Vector2 CCamera::ConvertPosition(Vector2 pos) {
	
	return Vector2(pos.x - camPosition.x, pos.y - camPosition.y);
}

void CCamera::UpdateCamPosition() {
	float left, top, right, bottom;
	positionController->GetBoundingBox(left, top, right, bottom);

	//SetCamPosition(Vector2((int)(left+right-camSize.x)/2, (int)(top+bottom - 2*camSize.y + 150) / 2));
	SetCamPosition(Vector2((int)(left + right - camSize.x) / 2, 710));
}

void CCamera::Update(DWORD dt) {
	
	//CGame* game = CGame::GetInstance();
	UpdateCamPosition();
	mMap->UpdateCamPosition(camPosition);
	
	
	
}

void CCamera::Render() {
	mMap->Render();
	
}
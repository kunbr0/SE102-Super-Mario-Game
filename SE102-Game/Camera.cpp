#include "Camera.h"



CCamera::CCamera() {
	this->camPosition = Vector2(0, 0);
	this->camSize = Vector2(640, 640);
	//mMap = NULL;
}
CCamera::~CCamera() {

}

void CCamera::InitPositionController(CGameObject* player) {
	this->positionController = player;
}

void CCamera::LoadMap() {
	/*mMap = new GameMap("Resources/new_world_1_1n.tmx");*/
	mMap = CGameMap().FromTMX("Resources/new_world_1_1n.tmx");
}

Vector2 CCamera::GetCamPosition() {
	return camPosition;
}

void CCamera::SetCamPosition(Vector2 pos) {
	camPosition = pos;
}

Vector2 CCamera::ConvertPosition(Vector2 pos) {
	return Vector2(pos.x - camPosition.x, pos.y - camPosition.y);
}

void CCamera::UpdateCamPosition() {
	float left, top, right, bottom;
	positionController->GetBoundingBox(left, top, right, bottom);

	SetCamPosition(Vector2((int)(left+right-camSize.x)/2, (int)(top+bottom - camSize.y) / 2));
}

void CCamera::Update(DWORD dt) {
	
	//CGame* game = CGame::GetInstance();
	UpdateCamPosition();
	mMap->UpdateCamPosition(camPosition);
	
	
	
}

void CCamera::Render() {
	mMap->Render();
	
}
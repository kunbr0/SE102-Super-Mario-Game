#include "Camera.h"



CCamera::CCamera() {
	
	

}
CCamera::~CCamera() {

}

void CCamera::LoadMap() {
	mMap = new GameMap("Resources/new_world_1_1n.tmx");
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

void CCamera::Update(DWORD dt) {
	//CGame* game = CGame::GetInstance();
	mMap->UpdateCamPosition(camPosition);
	float l, t, r, b;
	player->GetBoundingBox(l, t, r, b);


	SetCamPosition(Vector2(
		(int)(player->x - (750-(r-l))/2), 
		(int)(player->y - (525-(b-t))/2)
	));
	
	
}

void CCamera::Render() {
	mMap->Draw();
	Vector2 a = ConvertPosition(Vector2(player->x, player->y));
	player->Render(a);
}
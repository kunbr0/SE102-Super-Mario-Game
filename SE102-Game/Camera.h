#pragma once
#include "Transform.h"
#include "GameObject.h"
#include "GameMap.h"
#include "GameMap2.h"
class CCamera {

protected:
	
	Vector2 camPosition = Vector2(0.0f, 0.0f);
	Vector2 camSize; // (width,height) px
	Vector2 mapSize; // (width, height) px
	CGameObject* positionController; // who controls the camera position
	shared_ptr<CGameMap> mMap;
public:

	CCamera();
	~CCamera();
	Vector2 GetCamPosition();
	Vector2 GetCamSize() { return camSize; }
	void SetCamPosition(Vector2 pos);
	Vector2 ConvertPosition(Vector2 pos);
	
	void InitPositionController(CGameObject* a);

	void UpdateCamPosition();

	void LoadMap(std::string mapFilePath, vector<LPGAMEOBJECT>* objects);
	void Update(DWORD dt);
	void Render();
};
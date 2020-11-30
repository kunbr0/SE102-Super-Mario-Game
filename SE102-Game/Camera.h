#pragma once
#include "Transform.h"
#include "GameObject.h"
#include "GameMap2.h"

struct MapData
{
	int timeRemaining = 0;
	int score = 0;
	int playerLife = 1;
};


class CCamera {

protected:
	
	Vector2 camPosition = Vector2(0.0f, 0.0f);
	Vector2 camSize; // (width,height) px
	Vector2 mapSize; // (width, height) px
	CGameObject* positionController; // who controls the camera position
	CGameMap* mMap;
	MapData	mapData;
	

public:

	CCamera();
	~CCamera();
	Vector2 GetCamPosition();
	Vector2 GetCamSize() { return camSize; }
	void SetCamPosition(Vector2 pos);
	Vector2 ConvertPosition(Vector2 pos);
	
	void SetMapData(int=-1, int=-1, int=-1);
	void AdjustTimeRemaining(int);
	void AdjustScore(int);

	bool IsInCamera(Vector2 realPos, int outsideCam = 20);
	
	void InitPositionController(CGameObject* a);

	void UpdateCamPosition();
	

	void LoadMap(std::string, vector<LPGAMEOBJECT>* = NULL, vector<LPGAMEOBJECT>* = NULL, vector<LPGAMEOBJECT>* = NULL, vector<LPGAMEOBJECT>* = NULL);
	void Update(DWORD dt);
	void Render();
	void RenderDetailBoard();
};
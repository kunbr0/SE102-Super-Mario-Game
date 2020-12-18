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
	
	Vector2 camPosition ;
	Vector2 camSize; // (width,height) px
	Vector2 mapSize; // (width, height) px

	Vector2 LeftTopLimit = Vector2(0, 0);
	Vector2 RightBottomLimit = Vector2(0, 0);

	bool isLocked = true;

	CGameObject* positionController; // who controls the camera position
	CGameMap* mMap;
	MapData	mapData;
	

public:

	CCamera();
	~CCamera();
	Vector2 GetCamPosition();
	Vector2 GetCamSize() { return camSize; }
	Vector2 GetMapSize() { return mapSize; }
	void SetCamPosition(Vector2 pos);
	Vector2 ConvertPosition(Vector2 pos);
	
	void ChangeCamPosition(Vector2);
	void ChangeCamArea(Vector2 ar1, Vector2 ar2);

	void SetMapData(int=-1, int=-1, int=-1);
	void AdjustTimeRemaining(int);
	void AdjustScore(int);

	void RenderPausing();

	bool IsInCamera(Vector2 realPos, int outsideCam = 200);
	
	void InitPositionController(CGameObject* a);

	void UpdateCamPosition();
	
	
	// IntroScene
	void LoadMap(std::string, vector<LPGAMEOBJECT>* = NULL);

	// PlayScene
	void LoadMap(std::string, vector<LPGAMEOBJECT>*, vector<LPGAMEOBJECT>*, vector<LPGAMEOBJECT>* = NULL, vector<LPGAMEOBJECT>* = NULL);
	// SelectionScene
	void LoadMap(std::string, vector<LPGAMEOBJECT>*, std::unordered_map<std::string, CSelectionNode*>* );
	

	void Update(DWORD dt);
	void Render();
	void RenderDetailBoard();
};
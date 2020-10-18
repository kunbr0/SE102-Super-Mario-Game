#pragma once
#include "Transform.h"
#include "GameMap.h"
#include "Mario.h"
//#include "Game.h"

class CCamera {

protected:
	CMario* player; // Player controls the position of camera.
	GameMap* mMap;
	Vector2 camPosition = Vector2(0.0f, 0.0f);

public:

	CCamera();
	~CCamera();
	Vector2 GetCamPosition();
	void SetCamPosition(Vector2 pos);
	Vector2 ConvertPosition(Vector2 pos);
	CMario* GetPlayer() { return player; }
	void SetPlayer(CMario* obj) { player = obj; }

	void LoadMap();
	void Update(DWORD dt);
	void Render();
};
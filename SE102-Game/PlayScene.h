#pragma once
#include "Game.h"
#include "TextureManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Mario.h"
//#include "GameMap2.h"


class CPlayScene : public CScene
{
protected:
	CMario* player; // Player controls the position of camera.
	//shared_ptr<CGameMap> kMap;
	vector<LPGAMEOBJECT> objects;

	float standingY;


public:
	CPlayScene(std::string id, std::string filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	virtual bool LoadDataFromFile();
	/*virtual bool ParseTextures();
	virtual bool ParseSprites();*/


	CMario* GetPlayer() { return player; }
	void SetPlayer(CMario* obj) { player = obj; }

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};


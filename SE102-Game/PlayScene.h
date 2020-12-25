#pragma once
#include "Game.h"
#include "TextureManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Mario.h"
#include "Effect.h"
#include <map>

//#include "GameMap2.h"

struct SChangeCamData {
	Vector2 playerPos; 
	Vector2 LeftTopLimit;
	Vector2 RightBottomLimit;
};

class CPlayScene : public CScene
{

protected:
	bool isPausing = false;
	LPGAMEOBJECT player; // Player controls the position of camera.
	int playerLevel = 1;
	//shared_ptr<CGameMap> kMap;
	vector<LPGAMEOBJECT> staticObjects;
	vector<LPGAMEOBJECT> dynamicObjects;
	vector<LPGAMEOBJECT> dynamicObjectsBehindMap;
	vector<LPGAMEOBJECT> mainObjects;
	vector<LPGAMEOBJECT> highPriorityObjects;
	vector<LPGAMEOBJECT> tempObjects;
	
	vector<CEffect*> effects;

	vector<LPGAMEOBJECT> enemyBullets;

	//std::map<std::string, Vector2> miniPortals;

	float standingY;
	SChangeCamData changeCamData;


public:
	CPlayScene(std::string id, std::string, std::string);
	void SwitchPlayer(LPGAMEOBJECT newPlayer);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	virtual bool LoadDataFromFile();
	/*virtual bool ParseTextures();
	virtual bool ParseSprites();*/

	void SetPlayerLevel(int a) { playerLevel = a; }

	vector<LPGAMEOBJECT> GetEnemyBullets() { return enemyBullets; }
	bool GetPausingStatus() { return isPausing; }
	void TogglePausingMode();

	LPGAMEOBJECT GetPlayer() { return player; }
	void SetPlayer(LPGAMEOBJECT obj) { player = obj; }
	void PushDynamicObjects(LPGAMEOBJECT obj) { dynamicObjects.push_back(obj); }
	void PushTempObjects(LPGAMEOBJECT obj) { tempObjects.push_back(obj); }
	void PushHighPriorityObjects(LPGAMEOBJECT obj) { highPriorityObjects.push_back(obj); }
	void PushEffects(CEffect* eff) { effects.push_back(eff); }

	void HandleSceneTime(DWORD&);

	void UpdateEffects(DWORD);

	void DownLevelMario();
	void UpLevelMario();

	void HandleMarioDie();

	void SwitchToSelectionScene();

	CMario* GenerateMario(MarioType, Vector2);

	void PrepareChangeCameraArea(Vector2, Vector2, Vector2);
	void ChangeCameraArea();


	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};


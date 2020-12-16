#pragma once
#include "Game.h"
#include "Scene.h"
#include "GameObject.h"
#include <map>


class CSelectionScene : public CScene
{

protected:
	LPGAMEOBJECT player; // Player controls the position of camera.
	vector<LPGAMEOBJECT> selectionPortals;

public:
	CSelectionScene(std::string id, std::string filePath);
	virtual bool LoadDataFromFile();

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

};

class CSelectionSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CSelectionSceneKeyHandler(CScene* s) : CScenceKeyHandler(s) {};
};


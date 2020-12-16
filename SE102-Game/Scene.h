#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"
#include "Camera.h"
#define DEFAULT_TIME_SCALE		1


class CScene
{
protected:
	CKeyEventHandler* key_handler;
	std::string id;
	std::string sceneFilePath;
	CCamera sceneCamera;
	unsigned long timeScale;

public:
	CScene(std::string id, std::string filePath);

	CCamera* GetCamera() { return &sceneCamera; }

	CKeyEventHandler* GetKeyEventHandler() { return key_handler; }
	virtual bool LoadDataFromFile();
	virtual void Load() = 0;
	virtual void Unload() = 0;
	
	
	
	// Update
	virtual void Update(DWORD dt) = 0;
	void UpdateIfInCameraOrDisable(vector<LPGAMEOBJECT>*, DWORD, vector<LPGAMEOBJECT>*);
	void UpdateIfInCamera(vector<LPGAMEOBJECT>*, DWORD, vector<LPGAMEOBJECT>*);
	void UpdateTempObjsInCamera(vector<LPGAMEOBJECT>*, DWORD, vector<LPGAMEOBJECT>*);
	void PushBackToCalculateCollision(vector<LPGAMEOBJECT>*, vector<LPGAMEOBJECT>*);

	// Render
	virtual void Render() = 0;
	void RenderIfEnableAndInCamera(vector<LPGAMEOBJECT>*);

};
typedef CScene* LPSCENE;


class CScenceKeyHandler : public CKeyEventHandler
{
protected:
	CScene* scence;

public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CScenceKeyHandler(CScene* s) :CKeyEventHandler() { scence = s; }
};
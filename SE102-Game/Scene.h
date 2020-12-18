#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"
#include "Camera.h"
#define DEFAULT_TIME_SCALE		1


typedef void (*CallbackType)();


struct SClosingOpeningEffect {
	float currentBlackPortion = 0;
	const float totalBlackPortion = 1500;
	const float depreciation = 100;
	bool isOpening = true;
	bool isActive = false;

	CallbackType callback;
};

class CScene
{
protected:
	CKeyEventHandler* key_handler;
	std::string id;
	std::string type;
	std::string sceneFilePath;
	CCamera sceneCamera;
	unsigned long timeScale;

	SClosingOpeningEffect closingOpeningEffect;
	
	
public:
	CScene(std::string id, std::string filePath, std::string type);

	CCamera* GetCamera() { return &sceneCamera; }

	CKeyEventHandler* GetKeyEventHandler() { return key_handler; }
	std::string GetSceneType() { return type; }
	virtual bool LoadDataFromFile();
	virtual void Load() = 0;
	virtual void Unload() = 0;
	
	
	// Features
	void BeginOpeningEffect(CallbackType = nullptr);
	void BeginClosingEffect(CallbackType = nullptr);
	void CleanObjList(vector<LPGAMEOBJECT>&);
	
	// Update
	virtual void Update(DWORD dt) = 0;
	void UpdateIfInCameraOrDisable(vector<LPGAMEOBJECT>*, DWORD, vector<LPGAMEOBJECT>* = nullptr);
	void UpdateIfInCamera(vector<LPGAMEOBJECT>*, DWORD, vector<LPGAMEOBJECT>*);
	void UpdateTempObjsInCamera(vector<LPGAMEOBJECT>*, DWORD, vector<LPGAMEOBJECT>*);
	void PushBackToCalculateCollision(vector<LPGAMEOBJECT>*, vector<LPGAMEOBJECT>*);
	void ProcessBlackPortion(DWORD);

	// Render
	virtual void Render() = 0;
	void RenderIfEnableAndInCamera(vector<LPGAMEOBJECT>*);
	void RenderBlackEffect();

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
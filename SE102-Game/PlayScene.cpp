#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "TextureManager.h"
#include "Animation.h"

#include "Const.h"
#include "XmlReader/tinyxml.h"

#include "RedSmallMario.h"
#include "RedBigMario.h"
#include "RedRaccoonMario.h"
#include "FireMario.h"


CPlayScene::CPlayScene(std::string id, std::string filePath) :
	CScene(id, filePath)
{
	
	key_handler = new CPlayScenceKeyHandler(this);
	timeScale = DEFAULT_TIME_SCALE;
	
}


CMario* CPlayScene::GenerateMario(MarioType mType, Vector2 pos) {
	switch (mType)
	{
	case MarioType::RED_SMALL:
		return new CRedSmallMario(pos.x, pos.y);
	case MarioType::RED_BIG:
		return new CRedBigMario(pos.x, pos.y);
	case MarioType::RED_RACCON:
		return new CRedRaccoonMario(pos.x, pos.y);
	case MarioType::FIRE:
		return new CFireMario(pos.x, pos.y);
	default:
		return new CRedSmallMario(pos.x, pos.y);
	}
}


bool CPlayScene::LoadDataFromFile() {
	CScene::LoadDataFromFile();

	TiXmlDocument document(sceneFilePath.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return false;
	}

	TiXmlElement* root = document.RootElement();

	// Load Objects
	for (TiXmlElement* objs = root->FirstChildElement("objects"); objs != nullptr; objs = objs->NextSiblingElement("objects")) {
		// Mario
		for (TiXmlElement* mario = objs->FirstChildElement("mario"); mario != nullptr; mario = mario->NextSiblingElement("mario")) {
			if (player != NULL) break;
			int x = atoi(mario->Attribute("x"));
			int y = atoi(mario->Attribute("y"));
			int iType = 0; 
			mario->QueryIntAttribute("type", &iType);
			playerLevel = iType;
			SwitchPlayer(GenerateMario((MarioType)playerLevel, Vector2(x,y)));
		}

		for (TiXmlElement* mario = objs->FirstChildElement("enemyBullets"); mario != nullptr; mario = mario->NextSiblingElement("enemyBullets")) {
			int quantity = atoi(mario->Attribute("quantity"));
			for (int i = 0; i < quantity; i++) {
				enemyBullets.push_back(new CFireBullet(0,0,1));
			}
		}
	}
	
	std::string mapFilePath = root->Attribute("mapFilePath");
	sceneCamera.InitPositionController(player);
	sceneCamera.LoadMap(mapFilePath, &staticObjects, &dynamicObjects, &dynamicObjectsBehindMap, &tempObjects);
	sceneCamera.ChangeCamArea(Vector2(0, 0), Vector2(sceneCamera.GetMapSize().x, sceneCamera.GetMapSize().y - 520));

	return true;
}


void CPlayScene::Load()
{
	LoadDataFromFile();
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::HandleMarioDie() {
	if (((CMario*)player)->GetAction() == MarioAction::DIE) {
		if (playerLevel - 1 >= 0) playerLevel--;

		CMario* newMario = GenerateMario(
			(MarioType)(playerLevel),
			Vector2(player->x, player->y));
		newMario->ChangeAction(MarioAction::EXPLODE);
		SwitchPlayer(newMario);
	}
}

void CPlayScene::UpdateEffects(DWORD dt) {
	for (size_t i = 0; i < effects.size(); i++)
	{
		if (!effects[i]->GetActiveState()) {
			effects.erase(effects.begin() + i);
			i--;
		}
		else
			effects[i]->Update(dt);
	}

}

void CPlayScene::HandleSceneTime(DWORD& dt) {
	if (((CMario*)(player))->GetAction() == MarioAction::EXPLODE) {
		timeScale = 0;
	}
	else {
		timeScale = DEFAULT_TIME_SCALE;
	}

	dt = dt * timeScale;
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	
	if (player == NULL) return;

	HandleSceneTime(dt);

	vector<LPGAMEOBJECT> coObjects;
	
	PushBackToCalculateCollision(&coObjects, &staticObjects);

	UpdateIfInCamera(&dynamicObjects, dt, &coObjects);
	UpdateIfInCamera(&dynamicObjectsBehindMap, dt, &coObjects);
	
	PushBackToCalculateCollision(&coObjects, &dynamicObjects);
	PushBackToCalculateCollision(&coObjects, &dynamicObjectsBehindMap);

	HandleMarioDie();
	UpdateIfInCameraOrDisable(&mainObjects, dt, &coObjects);
	UpdateTempObjsInCamera(&tempObjects, dt, &mainObjects);

	UpdateIfInCameraOrDisable(&enemyBullets, dt, &mainObjects);
	UpdateEffects(dt);

	sceneCamera.Update(dt); // Update Map in Camera
}

void CPlayScene::Render()
{
	Vector2 camSize = sceneCamera.GetCamSize();

	RenderIfEnableAndInCamera(&dynamicObjectsBehindMap);


	sceneCamera.Render();

	RenderIfEnableAndInCamera(&staticObjects);
	/*for (int i = 0; i < staticObjects.size(); i++)
		staticObjects[i]->Render(sceneCamera.ConvertPosition(Vector2(staticObjects[i]->x, staticObjects[i]->y)));
	*/
	RenderIfEnableAndInCamera(&dynamicObjects);
	RenderIfEnableAndInCamera(&mainObjects);

	RenderIfEnableAndInCamera(&tempObjects);

	RenderIfEnableAndInCamera(&enemyBullets);
	
	

	for (size_t i = 0; i < effects.size(); i++)
	{
		Vector2 finalPos = sceneCamera.ConvertPosition(effects[i]->GetCurrentPosition());
		effects[i]->Render(finalPos);
	}
	sceneCamera.RenderDetailBoard();
}


void CPlayScene::ChangeCameraArea(Vector2 playerPos, Vector2 LeftTopLimit, Vector2 RightBottomLimit) {
	player->SetPosition(playerPos.x, playerPos.y);
	sceneCamera.ChangeCamArea(LeftTopLimit, RightBottomLimit);
}

/*
	Unload current scene
*/


void CPlayScene::Unload()
{
	for (int i = 0; i < staticObjects.size(); i++)
		delete staticObjects[i];

	staticObjects.clear();
	SetPlayer(NULL);

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::SwitchPlayer(LPGAMEOBJECT newPlayer) {
	if (!newPlayer) return;

	if (player != NULL) {
		float oldLeft, oldTop, oldRight, oldBottom;
		player->GetBoundingBox(oldLeft, oldTop, oldRight, oldBottom);
		float newLeft, newTop, newRight, newBottom;
		newPlayer->GetBoundingBox(newLeft, newTop, newRight, newBottom);
		newPlayer->y -= (newBottom - oldBottom);
		newPlayer->x -= (newRight - oldRight);
	}
	
	
	// Delete pointer of Old Mario in List Objects
	
	while (mainObjects.size() > 0) {
		delete mainObjects.at(0);
		mainObjects.erase(mainObjects.begin());
	}
	
	player = newPlayer;
	mainObjects.push_back(newPlayer);
	// Add Fire Bullet
	if (dynamic_cast<CFireMario*>(newPlayer)) {
		mainObjects.push_back(((CFireMario*)newPlayer)->GetBullet(0));
		mainObjects.push_back(((CFireMario*)newPlayer)->GetBullet(1));
	}

	sceneCamera.InitPositionController(newPlayer);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* currentPlayer = (CMario * )(((CPlayScene*)scence)->GetPlayer());
	CGame* gameInstance = CGame::GetInstance();

	currentPlayer->ProcessKeyboard(gameInstance->GenerateKeyboardEvent(KeyCode));
	switch (KeyCode)
	{
		case DIK_0:
			currentPlayer->Reset();
			break;
		case DIK_1: {
			((CPlayScene*)scence)->SwitchPlayer(
				new CRedSmallMario(currentPlayer->x, currentPlayer->y)
			);
		}	break;

		case DIK_2: {
			((CPlayScene*)scence)->SwitchPlayer(
				new CRedBigMario(currentPlayer->x, currentPlayer->y)
			);
		}	break;

		case DIK_3: {
			((CPlayScene*)scence)->SwitchPlayer(
				new CRedRaccoonMario(currentPlayer->x, currentPlayer->y)
			);
		}	break;
	
		case DIK_4: {
			((CPlayScene*)scence)->SwitchPlayer(
				new CFireMario(currentPlayer->x, currentPlayer->y)
			);
		}	break;
	}

}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* currentPlayer = (CMario*)(((CPlayScene*)scence)->GetPlayer());
	CGame* gameInstance = CGame::GetInstance();
	currentPlayer->ProcessKeyboard(gameInstance->GenerateKeyboardEvent(KeyCode, false, true));
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* gameInstance = CGame::GetInstance();
	CMario* currentPlayer = (CMario * )(((CPlayScene*)scence)->GetPlayer());
	if (currentPlayer == NULL)return;
	std::vector<int> UnOrderProcessKey = { DIK_A, DIK_S };
	std::vector<int> OrderProcessKey = { DIK_RIGHT, DIK_LEFT, DIK_DOWN };
	

	for (int i = 0; i < UnOrderProcessKey.size(); i++) {
		if (gameInstance->IsKeyDown(UnOrderProcessKey[i])) {
			currentPlayer->ProcessKeyboard(gameInstance->GenerateKeyboardEvent(UnOrderProcessKey[i], true));
		}
	}

	for (int i = 0; i < OrderProcessKey.size(); i++) {
		if (gameInstance->IsKeyDown(OrderProcessKey[i])) {
			currentPlayer->ProcessKeyboard(gameInstance->GenerateKeyboardEvent(OrderProcessKey[i], true));
			return;
		}
				
	}
	currentPlayer->ProcessKeyboard(gameInstance->GenerateKeyboardEvent(-1, true));
	
	
	
}
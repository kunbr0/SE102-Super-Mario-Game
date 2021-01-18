
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
#include "GoldenBrick.h"

#define VELOCITY_X_ENEMY_FIRE_BULLET			0.3625f 


CPlayScene::CPlayScene(std::string id, std::string filePath, std::string type) :
	CScene(id, filePath, type)
{

	key_handler = new CPlayScenceKeyHandler(this);
	timeScale = DEFAULT_TIME_SCALE;
	LPGAMEOBJECT cameraLimitController = NULL;

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
			SwitchPlayer(GenerateMario((MarioType)playerLevel, Vector2(x, y)));
		}

		for (TiXmlElement* mario = objs->FirstChildElement("enemyBullets"); mario != nullptr; mario = mario->NextSiblingElement("enemyBullets")) {
			int quantity = atoi(mario->Attribute("quantity"));
			for (int i = 0; i < quantity; i++) {
				enemyBullets.push_back(new CFireBullet(0, 0, 1, 1));
			}
		}
	}

	std::string mapFilePath = root->Attribute("mapFilePath");

	sceneCamera.LoadMap(mapFilePath, &cameraLimitController, &staticObjects, &dynamicObjects, &dynamicObjectsBehindMap, &tempObjects);
	if (cameraLimitController == NULL)
		sceneCamera.InitPositionController(player);
	else sceneCamera.InitPositionController(cameraLimitController);
	sceneCamera.InitMario(player);
	// 520 is the black of World1-1, 96 is World1-2
	sceneCamera.ChangeCamArea(Vector2(0, 0), Vector2(sceneCamera.GetMapSize().x, sceneCamera.GetMapSize().y -520));

	return true;
}


void CPlayScene::Load()
{
	LoadDataFromFile();
	BeginOpeningEffect();
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::DownLevelMario() {

	if (playerLevel - 1 >= 0) playerLevel--;

	CMario* newMario = GenerateMario(
		(MarioType)(playerLevel),
		Vector2(player->x, player->y));
	newMario->BeginUntouchable();
	SwitchPlayer(newMario);

}

void CPlayScene::UpLevelMario() {

	if (playerLevel + 1 <= 3) playerLevel++;

	CMario* newMario = GenerateMario(
		(MarioType)(playerLevel),
		Vector2(player->x, player->y));
	newMario->BeginUntouchable();
	SwitchPlayer(newMario);

}

void CPlayScene::HandleMarioDie() {

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
	if (((CMario*)(player))->GetAction() == MarioAction::EXPLODE ||
		((CMario*)(player))->GetAction() == MarioAction::UPGRADE_LV) {
		if (((CMario*)(player))->IsReadyToChangeAction()) {
			timeScale = DEFAULT_TIME_SCALE;
			if (((CMario*)(player))->GetAction() == MarioAction::EXPLODE)
				DownLevelMario();
			else
				UpLevelMario();
		}
		else
		{
			UpdateIfInCameraOrDisable(&enemyBullets, dt, &enemyBullets);
			timeScale = 0;
		}
	}
	else {
		timeScale = DEFAULT_TIME_SCALE;
	}

	dt = dt * timeScale;
}

void CPlayScene::SwitchToSelectionScene() {
	if (closingOpeningEffect.isActive == false) {
		BeginClosingEffect([]() {
			CGame::GetInstance()->SwitchScene("world1-selection");
		});
	}
		

}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	ProcessBlackPortion(dt);
	if (player == NULL || isPausing) return;

	if (((CMario*)(player))->GetAction() == MarioAction::DIE) {
		
		player->Update(dt, &mainObjects);
		return;
	}

	HandleSceneTime(dt);

	vector<LPGAMEOBJECT> coObjects;

	PushBackToCalculateCollision(&coObjects, &staticObjects);

	UpdateIfInCamera(&dynamicObjects, dt, &coObjects);
	UpdateIfInCamera(&dynamicObjectsBehindMap, dt, &coObjects);


	PushBackToCalculateCollision(&coObjects, &dynamicObjects);
	PushBackToCalculateCollision(&coObjects, &dynamicObjectsBehindMap);


UpdateIfInCameraOrDisable(&mainObjects, dt, &coObjects);
PushBackToCalculateCollision(&coObjects, &mainObjects);

UpdateTempObjsInCamera(&highPriorityObjects, dt, &coObjects);

UpdateTempObjsInCamera(&tempObjects, dt, &mainObjects);

UpdateIfInCameraOrDisable(&enemyBullets, dt, &mainObjects);
UpdateEffects(dt);

if (cameraLimitController != NULL)
cameraLimitController->Update(dt, nullptr);

sceneCamera.Update(dt); // Update Map in Camera

}

void CPlayScene::Render()
{

	Vector2 camSize = sceneCamera.GetCamSize();

	if (((CMario*)(player))->GetAction() == MarioAction::GETTING_INTO_THE_HOLE)
		RenderIfEnableAndInCamera(&mainObjects);
	RenderIfEnableAndInCamera(&dynamicObjectsBehindMap);

	sceneCamera.Render();
	
	RenderIfEnableAndInCamera(&staticObjects);
	/*for (int i = 0; i < staticObjects.size(); i++)
		staticObjects[i]->Render(sceneCamera.ConvertPosition(Vector2(staticObjects[i]->x, staticObjects[i]->y)));
	*/
	RenderIfEnableAndInCamera(&dynamicObjects);

	if (((CMario*)(player))->GetAction() != MarioAction::GETTING_INTO_THE_HOLE)
		RenderIfEnableAndInCamera(&mainObjects);

	RenderIfEnableAndInCamera(&tempObjects);
	RenderIfEnableAndInCamera(&highPriorityObjects);

	RenderIfEnableAndInCamera(&enemyBullets);



	for (size_t i = 0; i < effects.size(); i++)
	{
		Vector2 finalPos = sceneCamera.ConvertPosition(effects[i]->GetCurrentPosition());
		effects[i]->Render(finalPos);
	}
	sceneCamera.RenderDetailBoard();
	RenderBlackEffect();

	if (isPausing) sceneCamera.RenderPausing();
}


void CPlayScene::PrepareChangeCameraArea(Vector2 playerPos, Vector2 LeftTopLimit, Vector2 RightBottomLimit) {
	changeCamData.playerPos = playerPos;
	changeCamData.LeftTopLimit = LeftTopLimit;
	changeCamData.RightBottomLimit = RightBottomLimit;
}


void CPlayScene::ChangeCameraArea() {
	player->SetPosition(changeCamData.playerPos);
	sceneCamera.ChangeCamArea(changeCamData.LeftTopLimit, changeCamData.RightBottomLimit);
}



/*
	Unload current scene
*/

void CPlayScene::TogglePausingMode() {
	isPausing = !isPausing;
}


void CPlayScene::Unload()
{
	CleanObjList(staticObjects);
	CleanObjList(dynamicObjects);
	CleanObjList(dynamicObjectsBehindMap);
	CleanObjList(mainObjects);
	CleanObjList(tempObjects);
	//CleanObjList(highPriorityObjects);
	CleanObjList(enemyBullets);

	/*for (int i = 0; i < staticObjects.size(); i++)
		delete staticObjects[i];
	staticObjects.clear();*/
	SetPlayer(NULL);

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::ChangeGoldenBricksInCameraToShowCoinState() {
	for (int i = 0; i < staticObjects.size(); i++) {
		if (sceneCamera.IsInCamera(staticObjects[i]->GetPosition())){
			if (dynamic_cast<CGoldenBrick*>(staticObjects[i])) {
				((CGoldenBrick*)(staticObjects[i]))->ChangeState(EBlockState::SHOW_HIDDEN_COIN);
			}
		}
	}
}

void CPlayScene::BeginVibratingCamera(DWORD totalTime) {
	sceneCamera.BeginVibrating(totalTime);
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
	sceneCamera.InitMario(newPlayer);
	if (cameraLimitController == NULL)
		sceneCamera.InitPositionController(newPlayer);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	if (((CPlayScene*)scence)->GetPausingStatus() && KeyCode != DIK_ESCAPE) return;
	CMario* currentPlayer = (CMario*)(((CPlayScene*)scence)->GetPlayer());
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
		((CPlayScene*)scence)->SetPlayerLevel(0);
	}	break;

	case DIK_2: {
		((CPlayScene*)scence)->SwitchPlayer(
			new CRedBigMario(currentPlayer->x, currentPlayer->y)
		);
		((CPlayScene*)scence)->SetPlayerLevel(1);
	}	break;

	case DIK_3: {
		((CPlayScene*)scence)->SwitchPlayer(
			new CRedRaccoonMario(currentPlayer->x, currentPlayer->y)
		);
		((CPlayScene*)scence)->SetPlayerLevel(2);
	}	break;

	case DIK_4: {
		((CPlayScene*)scence)->SwitchPlayer(
			new CFireMario(currentPlayer->x, currentPlayer->y)
		);
		((CPlayScene*)scence)->SetPlayerLevel(3);
	}	break;

	case DIK_ESCAPE: {
		((CPlayScene*)scence)->TogglePausingMode();
	}	break;
	}

}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	if (((CPlayScene*)scence)->GetPausingStatus()) return;
	CMario* currentPlayer = (CMario*)(((CPlayScene*)scence)->GetPlayer());
	CGame* gameInstance = CGame::GetInstance();
	currentPlayer->ProcessKeyboard(gameInstance->GenerateKeyboardEvent(KeyCode, false, true));
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	if (((CPlayScene*)scence)->GetPausingStatus()) return;
	CGame* gameInstance = CGame::GetInstance();
	CMario* currentPlayer = (CMario*)(((CPlayScene*)scence)->GetPlayer());
	if (currentPlayer == NULL)return;
	std::vector<int> UnOrderProcessKey = { DIK_A, DIK_S };
	std::vector<int> OrderProcessKey = { DIK_LEFT,DIK_RIGHT, DIK_DOWN };


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
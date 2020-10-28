#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "TextureManager.h"
#include "Animation.h"
#include "Portal.h"
#include "Const.h"
#include "XmlReader/tinyxml.h"

#include "Brick.h"
#include "Goomba.h"
#include "Koopas.h"
#include "RedSmallMario.h"
#include "RedBigMario.h"
#include "RedRaccoonMario.h"
#include "FireMario.h"

using namespace std;

CPlayScene::CPlayScene(std::string id, std::string filePath) :
	CScene(id, filePath)
{
	
	key_handler = new CPlayScenceKeyHandler(this);
	
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024




bool CPlayScene::LoadDataFromFile() {
	TiXmlDocument document(sceneFilePath.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return false;
	}

	TiXmlElement* root = document.RootElement();



	// Load Texture
	for (TiXmlElement* textures = root->FirstChildElement("textures"); textures != nullptr; textures = textures->NextSiblingElement("textures")) {
		for (TiXmlElement* texture = textures->FirstChildElement("texture"); texture != nullptr; texture = texture->NextSiblingElement("texture")) {
			std::string id = texture->Attribute("id");
			std::string filePath = texture->Attribute("filePath");
			auto transColor = texture->Attribute("transColor");
			if (transColor != NULL) {
				auto rgb = split(transColor, ",");
				CTextures::GetInstance()->Add(id, ToLPCWSTR(filePath), D3DCOLOR_XRGB(atoi(rgb[0].c_str()), atoi(rgb[1].c_str()), atoi(rgb[2].c_str())));
			}
			else {
				CTextures::GetInstance()->Add(id, ToLPCWSTR(filePath), D3DXCOLOR());
			}
			
		}
	}

	// Load Sprite
	for (TiXmlElement* sprites = root->FirstChildElement("sprites"); sprites != nullptr; sprites = sprites->NextSiblingElement("sprites")) {
		for (TiXmlElement* sprite = sprites->FirstChildElement("sprite"); sprite != nullptr; sprite = sprite->NextSiblingElement("sprite")) {
			//std::string texId = sprite->Attribute("texId");
			std::string filePath = sprite->Attribute("filePath");
			CSprites::GetInstance()->LoadSpriteFromFile(filePath);
		}
	}

	// Load Animations
	for (TiXmlElement* animations = root->FirstChildElement("animations"); animations != nullptr; animations = animations->NextSiblingElement("animations")) {
		for (TiXmlElement* animation = animations->FirstChildElement("animation"); animation != nullptr; animation = animation->NextSiblingElement("animation")) {
			std::string idSet = animation->Attribute("idSet");
			std::string filePath = animation->Attribute("filePath");
			CAnimations::GetInstance()->LoadAnimationsFromFile(filePath, idSet);

		}
	}

	// Load Objects
	for (TiXmlElement* objs = root->FirstChildElement("objects"); objs != nullptr; objs = objs->NextSiblingElement("objects")) {
		// Mario
		for (TiXmlElement* mario = objs->FirstChildElement("mario"); mario != nullptr; mario = mario->NextSiblingElement("mario")) {
			if (player != NULL) break;
			int x = atoi(mario->Attribute("x"));
			int y = atoi(mario->Attribute("y"));
			std::string type = std::string(mario->Attribute("type"));
			if(type == "red-small")	SwitchPlayer(new CRedSmallMario(x, y));
			else if (type == "red-big")	SwitchPlayer(new CRedBigMario(x, y));
			else if (type == "red-raccoon")	SwitchPlayer(new CRedRaccoonMario(x, y));
			else if (type == "red-big")	SwitchPlayer(new CFireMario(x, y));
			// Default
			else SwitchPlayer(new CRedSmallMario(x, y));
		}
		
		// Goomba
		for (TiXmlElement* goomba = objs->FirstChildElement("goomba"); goomba != nullptr; goomba = goomba->NextSiblingElement("goomba")) {
			int x = atoi(goomba->Attribute("x"));
			int y = atoi(goomba->Attribute("y"));
			LPGAMEOBJECT a = new CGoomba(x, y);
			dynamicObjects.push_back(a);
		}
	}
	
	std::string mapFilePath = root->Attribute("mapFilePath");
	sceneCamera.InitPositionController(player);
	sceneCamera.LoadMap(mapFilePath, &staticObjects);
	return true;
}


void CPlayScene::Load()
{

	LoadDataFromFile();
	
	


	

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	
	
	

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < staticObjects.size(); i++)
	{
		coObjects.push_back(staticObjects[i]);
	}

	for (size_t i = 0; i < dynamicObjects.size(); i++)
	{
		dynamicObjects[i]->Update(dt, &coObjects);
	}
	/*player->Update(dt, &coObjects);*/

	//kMap->Update(dt);

	sceneCamera.Update(dt); // Update Map in Camera
	
}

void CPlayScene::Render()
{
	Vector2 camSize = sceneCamera.GetCamSize();

	sceneCamera.Render();
	for (int i = 0; i < staticObjects.size(); i++)
		staticObjects[i]->Render(sceneCamera.ConvertPosition(Vector2(staticObjects[i]->x, staticObjects[i]->y)));
	
	for (int i = 0; i < dynamicObjects.size(); i++)
		if (!dynamicObjects[i]->isDisable) {
			Vector2 finalPos = sceneCamera.ConvertPosition(Vector2(dynamicObjects[i]->x, dynamicObjects[i]->y));
			if (finalPos.x > 0 && finalPos.x < camSize.x && finalPos.y > 0 && finalPos.y < camSize.y)
				dynamicObjects[i]->Render(finalPos);
			else
				if(!dynamic_cast<CMario*>(dynamicObjects[i]))
					dynamicObjects[i]->isDisable = true;
		}
			

	//player->Render(sceneCamera.ConvertPosition(Vector2(player->x, player->y)));
	/*Vector2 a = sceneCamera.ConvertPosition(Vector2(player->x, player->y));
	player->Render(a);*/

	//kMap->Render();
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
		newPlayer->y -= newBottom - oldBottom;
		newPlayer->x -= newRight - oldRight;
	}
	
	
	// Delete pointer of Old Mario in List Objects
	
	while (dynamicObjects.size() > 0) {
		delete dynamicObjects.at(0);
		dynamicObjects.erase(dynamicObjects.begin());
	}
	
	player = newPlayer;
	dynamicObjects.push_back(newPlayer);
	// Add Fire Bullet
	if (dynamic_cast<CFireMario*>(newPlayer)) {
		dynamicObjects.push_back(((CFireMario*)newPlayer)->GetBullet(0));
		dynamicObjects.push_back(((CFireMario*)newPlayer)->GetBullet(1));
	}

	sceneCamera.InitPositionController(newPlayer);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* currentPlayer = (CMario * )(((CPlayScene*)scence)->GetPlayer());
	
	switch (KeyCode)
	{
		case DIK_X:
			currentPlayer->SetState(MARIO_STATE_JUMP_X);
			break;
		case DIK_S:
			currentPlayer->SetState(MARIO_STATE_JUMP_S);
			break;
		case DIK_Q:
			currentPlayer->SetState(MARIO_STATE_RACCOON_FLY);
			break;
		case DIK_A:
			currentPlayer->SetState(MARIO_STATE_PRESS_A);
			break;
		case DIK_Z:
			currentPlayer->SetState(MARIO_STATE_PRESS_Z);
			break;

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

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = (CMario * )(((CPlayScene*)scence)->GetPlayer());

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_A))
		mario->SetState(MARIO_STATE_PRESS_A);
	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	else
		mario->SetState(MARIO_STATE_NONE_PRESS_KEY);
}
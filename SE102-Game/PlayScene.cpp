#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "TextureManager.h"
#include "AnimationSet.h"
#include "Portal.h"
#include "Const.h"
#include "XmlReader/tinyxml.h"


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
			CAnimationSets::GetInstance()->LoadAnimationFromFile(filePath, idSet);

		}
	}

	// Load Objects
	for (TiXmlElement* objs = root->FirstChildElement("objects"); objs != nullptr; objs = objs->NextSiblingElement("objects")) {
		for (TiXmlElement* mario = objs->FirstChildElement("mario"); mario != nullptr; mario = mario->NextSiblingElement("mario")) {
			if (player != NULL) break;
			int x = atoi(mario->Attribute("x"));
			int y = atoi(mario->Attribute("y"));
			CMario* mar = new CFireMario(x, y);
			player = mar;
			//objects.push_back(mar);
		}
	}
	
	std::string mapFilePath = root->Attribute("mapFilePath");
	sceneCamera.InitPositionController(player);
	sceneCamera.LoadMap(mapFilePath, &objects);
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
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	/*for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}*/
	player->Update(dt, &coObjects);

	//kMap->Update(dt);

	sceneCamera.Update(dt); // Update Map in Camera
	
}

void CPlayScene::Render()
{
	/*for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
	float pX, pY, pVX, pVY;
	player->GetPosition(pX, pY);
	player->GetSpeed(pVX, pVY);
	
	std::string txDetails = 
		"(" + std::to_string((int)pX) + "," + std::to_string((int)(pY)) + ")" + "\n" +
		"Type: " + std::to_string(player->GetType()) 
		+ "\nLevel: " + std::to_string(player->GetLevel());
	 
	CGame::GetInstance()->KDrawBoardDetails(10, 10, txDetails.c_str());*/

	sceneCamera.Render();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render(sceneCamera.ConvertPosition(Vector2(objects[i]->x, objects[i]->y)));
	player->Render(sceneCamera.ConvertPosition(Vector2(player->x, player->y)));
	/*Vector2 a = sceneCamera.ConvertPosition(Vector2(player->x, player->y));
	player->Render(a);*/

	//kMap->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	SetPlayer(NULL);

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::SwitchPlayer(CMario* newPlayer) {
	float oldLeft, oldTop, oldRight, oldBottom;
	player->GetBoundingBox(oldLeft, oldTop, oldRight, oldBottom);
	float newLeft, newTop, newRight, newBottom;
	newPlayer->GetBoundingBox(newLeft, newTop, newRight, newBottom);
	newPlayer->y -= newBottom - oldBottom;
	newPlayer->x -= newRight - oldRight;
	if (!newPlayer) return;
	// Delete pointer of Old Mario in List Objects
	for (int i = 0; i < objects.size(); i++) {
		if (dynamic_cast<CMario*>(objects.at(i))) {
			objects.erase(objects.begin()+i);
		}
	}
	delete player;
	player = newPlayer;
	objects.push_back(newPlayer);
	sceneCamera.InitPositionController(newPlayer);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* currentPlayer = ((CPlayScene*)scence)->GetPlayer();
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

	case DIK_0:
		currentPlayer->Reset();
		break;
	case DIK_1: {
		((CPlayScene*)scence)->SwitchPlayer(
			new CRedMario(currentPlayer->x, currentPlayer->y)
		);
	}
		break;
	case DIK_2: {
		((CPlayScene*)scence)->SwitchPlayer(
			new CFireMario(currentPlayer->x, currentPlayer->y)
		);
	}
		break;
	case DIK_3:
		break;
	case DIK_4:
		break;
	case DIK_MINUS:
		currentPlayer->SetLevel(currentPlayer->GetLevel()-1);
		break;
	case DIK_EQUALS:
		currentPlayer->SetLevel(currentPlayer->GetLevel()+1);
		break;

	}

	

}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

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
#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "TextureManager.h"
#include "AnimationSet.h"
#include "Portal.h"
#include "Const.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
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


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	std::string texID = tokens[0] ;
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	std::string ID = (tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	string texID = (tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	std::string ani_id = (tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		std::string sprite_id = (tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	std::string ani_set_id = (tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		std::string ani_id = (tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	std::string ani_set_id = (tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (GetPlayer() != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		//player = (CMario*)obj;
		SetPlayer((CMario*)obj);
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	CAnimationSet* a = CAnimationSets::GetInstance()->Get("mario");
	GetPlayer()->SetAnimationSet(a);
	objects.push_back(obj);
}

void CPlayScene::Load()
{
	
	CTextures::GetInstance()->Add("tex-mario", L"Assets/Sprites/mario.png", D3DCOLOR());
	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"Textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	CSprites::GetInstance()->LoadSpriteFromFile("Assets/Sprites//SpriteDatabases/MarioDB.xml");
	CAnimationSets::GetInstance()->LoadAnimationFromFile("Assets/Animations/MarioAnim.xml", "mario");
	

	
	
	CGameObject* obj = NULL;
	obj = new CMario(800, 400);
	SetPlayer((CMario*)obj);
	objects.push_back(obj);
	sceneCamera.InitPositionController(player);
	sceneCamera.LoadMap();

	
	//kMap = CGameMap().FromTMX("Resources/new_world_1_1n.tmx");

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	
	

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}


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
	Vector2 a = sceneCamera.ConvertPosition(Vector2(player->x, player->y));
	player->Render(a);
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

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_X:
		mario->SetState(MARIO_STATE_JUMP_X);
		break;
	case DIK_S:
		mario->SetState(MARIO_STATE_JUMP_S);
		break;
	case DIK_A:
		mario->Reset();
		break;

	case DIK_1:
		mario->SetType(1);
		break;
	case DIK_2:
		mario->SetType(2);
		break;
	case DIK_3:
		mario->SetType(3);
		break;
	case DIK_4:
		mario->SetType(4);
		break;
	case DIK_MINUS:
		mario->SetLevel(mario->GetLevel()-1);
		break;
	case DIK_EQUALS:
		mario->SetLevel(mario->GetLevel()+1);
		break;

	}

	

}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	else
		mario->SetState(MARIO_STATE_IDLE);
}
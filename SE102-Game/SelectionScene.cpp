#include "SelectionScene.h"
#include "SelectionSceneMario.h"

CSelectionScene::CSelectionScene(std::string id, std::string filePath) :
	CScene(id, filePath)
{

	key_handler = new CSelectionSceneKeyHandler(this);
	timeScale = DEFAULT_TIME_SCALE;

}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/






void CSelectionScene::Load()
{
	LoadDataFromFile();
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

}

void CSelectionScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	
}

void CSelectionScene::Render()
{
	sceneCamera.Render();
	for (int i = 0; i < selectionPortals.size(); i++)
		selectionPortals[i]->Render(sceneCamera.ConvertPosition(Vector2(selectionPortals[i]->x, selectionPortals[i]->y)));

	player->Render(sceneCamera.ConvertPosition(Vector2(player->x, player->y)));
	
	sceneCamera.RenderDetailBoard();
}

/*
	Unload current scene
*/


void CSelectionScene::Unload()
{
	
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CSelectionSceneKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	
	switch (KeyCode)
	{
	case DIK_W:
		CGame::GetInstance()->SwitchScene("world1-1");
		break;
	}

}


bool CSelectionScene::LoadDataFromFile() {
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
			player = new CSelectionSceneMario(Vector2(x, y));
		}

	}
	std::string mapFilePath = root->Attribute("mapFilePath");
	sceneCamera.InitPositionController(player);
	sceneCamera.LoadMap(mapFilePath, &selectionPortals);

	sceneCamera.ChangeCamArea(Vector2(0, 0), Vector2(sceneCamera.GetMapSize().x, sceneCamera.GetMapSize().y));
	return true;
}
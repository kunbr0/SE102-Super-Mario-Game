#include "Scene.h"
#include "Mario.h"

CScene::CScene(std::string id, std::string filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
	sceneCamera = CCamera();
}




void CScene::UpdateTempObjsInCamera(vector<LPGAMEOBJECT>* objList, DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	for (size_t i = 0; i < objList->size(); i++)
	{
		if (!objList->at(i)->isTemp) {
			objList->erase(objList->begin() + i);
		}
		else if (sceneCamera.IsInCamera(Vector2(objList->at(i)->x, objList->at(i)->y)))
			objList->at(i)->Update(dt, coObjects);
		/*else
			if (!dynamic_cast<CMario*>(objList->at(i)))
				objList->at(i)->isDisable = true;*/
	}
}


void CScene::UpdateIfInCameraOrDisable(vector<LPGAMEOBJECT>* objList, DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	for (size_t i = 0; i < objList->size(); i++)
	{
		if (!(dynamic_cast<CMario*>(objList->at(i))) && !sceneCamera.IsInCamera(Vector2(objList->at(i)->x, objList->at(i)->y)))
			objList->at(i)->isDisable = true;
		else
			objList->at(i)->Update(dt, coObjects);
	}
}

void CScene::UpdateIfInCamera(vector<LPGAMEOBJECT>* objList, DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	for (size_t i = 0; i < objList->size(); i++)
	{
		if (sceneCamera.IsInCamera(Vector2(objList->at(i)->x, objList->at(i)->y)))
			objList->at(i)->Update(dt, coObjects);
	}
}

void CScene::PushBackToCalculateCollision(vector<LPGAMEOBJECT>* storageList, vector<LPGAMEOBJECT>* inputList) {
	for (size_t i = 0; i < inputList->size(); i++)
	{
		storageList->push_back(inputList->at(i));
	}
}



void CScene::RenderIfEnableAndInCamera(vector<LPGAMEOBJECT>* objList) {
	for (int i = 0; i < objList->size(); i++) {
		LPGAMEOBJECT obj = objList->at(i);
		if (!obj->isDisable) {
			if (sceneCamera.IsInCamera(obj->GetPosition())) {
				Vector2 finalPos = sceneCamera.ConvertPosition(obj->GetPosition());
				obj->Render(finalPos);
			}
		}
	}
		
}


bool CScene::LoadDataFromFile() {
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

}
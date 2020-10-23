#include "Scene.h"

CScene::CScene(std::string id, std::string filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
	sceneCamera = CCamera();
}
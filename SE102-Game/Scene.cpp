#include "Scene.h"

CScene::CScene(int id, std::string filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
	sceneCamera = CCamera();
}
#include "AnimationSet.h"



CAnimationSets* CAnimationSets::__instance = NULL;


CAnimationSets::CAnimationSets()
{

}

CAnimationSets* CAnimationSets::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimationSets();
	return __instance;
}

LPANIMATION_SET CAnimationSets::Get(std::string id)
{
	LPANIMATION_SET ani_set = animation_sets[id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n", id);

	return ani_set;
}

void CAnimationSets::Add(std::string id, LPANIMATION_SET ani_set)
{
	animation_sets[id] = ani_set;
}


bool CAnimationSets::LoadAnimationFromFile(std::string filePath, std::string setId) {
	CAnimationSet* aniSet = new CAnimationSet();

	OutputDebugStringW(ToLPCWSTR(filePath.c_str()));
	TiXmlDocument document(filePath.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return false;
	}

	TiXmlElement* root = document.RootElement();
	TiXmlElement* info = root->FirstChildElement();

	string gameObjectID = info->Attribute("gameObjectId");
	string textureID = info->Attribute("textureId");

	OutputDebugStringW(ToLPCWSTR("Gameobject id: " + gameObjectID + '\n'));
	OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));

	for (TiXmlElement* node = info->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
	{
		string aniId = node->Attribute("aniId");
		float frameTime;
		node->QueryFloatAttribute("frameTime", &frameTime);
		string name = node->Attribute("name");
		OutputDebugStringW(ToLPCWSTR(aniId + ':' + to_string(frameTime) + ':' + name + '\n'));
		
		CAnimation* animation = new CAnimation(frameTime);
		
		// Sprite ref
		for (TiXmlElement* sprNode = node->FirstChildElement(); sprNode != nullptr; sprNode = sprNode->NextSiblingElement())
		{
			string spriteId = sprNode->Attribute("id");
			
			float detailFrameTime;
			sprNode->QueryFloatAttribute("frameTime", &detailFrameTime);
			
			animation->Add(spriteId, detailFrameTime);

			//OutputDebugStringW(ToLPCWSTR("|--" + id + ':' + to_string(detailFrameTime) + '\n'));
		}

		CAnimations::GetInstance()->Add(aniId, animation);
		aniSet->push_back(animation);
		
	}
	CAnimationSets::GetInstance()->Add(setId, aniSet);

	return true;
}
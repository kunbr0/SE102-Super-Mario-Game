#pragma once
#include "Animation.h"
#include "XmlReader/tinyxml.h"
#include "SpriteManager.h"

class CAnimationSets
{
	static CAnimationSets* __instance;

	unordered_map<std::string, LPANIMATION_SET> animation_sets;

public:
	CAnimationSets();
	void Add(std::string id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(std::string id);

	bool LoadAnimationFromFile(std::string filePath, std::string setId);

	static CAnimationSets* GetInstance();
};

#pragma once

#include "Sprite.h"
#include "TextureManager.h"

/*
	Manage sprite database
*/
class CSprites
{
	static CSprites* __instance;

	unordered_map<std::string, LPSPRITE> sprites;

public:
	void Add(std::string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(std::string id);
	void Clear();

	bool LoadSpriteFromFile(std::string filePath);

	static CSprites* GetInstance();
};
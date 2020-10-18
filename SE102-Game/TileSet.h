#pragma once
#include "Utils.h"
#include "Transform.h"
#include "./XmlReader/tinyxml.h"
#include "./XmlReader/tinystr.h"
#include <memory>
#include <unordered_map>

class CTileSet
{
	int firstgid;

	D3DXVECTOR2 tileSize;

	int tileCount;
	int columns;

	LPDIRECT3DTEXTURE9 texture;

	unordered_map<int, std::shared_ptr<RECT>> blocks;

public:
	CTileSet(int firstgid, D3DXVECTOR2 tileSize, int tileCount, int columns, string imgPath);
	CTileSet(TiXmlElement* data, string xmlPath);

	int GetFirstGID();

	std::shared_ptr<RECT> GetBlockBoundingBox(int id);

	void Draw(int gid, float x, float y, Transform& transform, D3DCOLOR overlay = D3DCOLOR_ARGB(255, 255, 255, 255));

	~CTileSet();
};

typedef CTileSet* TileSet;
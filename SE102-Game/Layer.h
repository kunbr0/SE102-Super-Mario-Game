#pragma once
#include "Utils.h"
#include "./XmlReader/tinyxml.h"
#include "./XmlReader/tinystr.h"

class CLayer
{
	int id;

	int width;
	int height;

	int** tiles;
public:
	CLayer();
	CLayer(TiXmlElement* data);

	bool Hidden = false;

	int GetTileID(int x, int y);

	~CLayer();
};

typedef CLayer* Layer;

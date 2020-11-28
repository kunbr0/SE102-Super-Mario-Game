#pragma once
#include "Utils.h"
#include "TileSet.h"
#include "MapLayer.h"
#include "./XmlReader/tinyxml.h"
#include "./XmlReader/tinystr.h"
#include<map>
#include "GameObject.h"


class CGameMap
{
	int width;
	int height;
	int tileWidth;
	int tileHeight;
	Vector2 camPosition;
	//shared_ptr<Camera> camera;

	map<int, shared_ptr<CTileSet>> tilesets;
	vector<shared_ptr<CMapLayer>> layers;

public:
	CGameMap();
	CGameMap(int width, int height, int tileWidth, int tileHeight);


	virtual Vector2 GetBound();

	virtual shared_ptr<CTileSet> GetTileSetByTileID(int id);

	virtual void AddTileSet(int firstgid, shared_ptr<CTileSet> tileSet);

	virtual void AddLayer(shared_ptr<CMapLayer> layer);

	void UpdateCamPosition(Vector2 newPos);
	Vector2 ConvertToPositionInCam(Vector2 oldPos);

	virtual void Update(int dt);

	virtual void Render(float bottomMargin = 0);

	virtual void GetMapSize(Vector2 &out);

	static shared_ptr<CGameMap> FromTMX(string, vector<LPGAMEOBJECT>* = NULL, vector<LPGAMEOBJECT>* = NULL, vector<LPGAMEOBJECT>* = NULL);
	~CGameMap();
};

typedef CGameMap* LPGameMap;

#pragma once
#include "Utils.h"
#include "TileSet.h"
#include "MapLayer.h"
#include "./XmlReader/tinyxml.h"
#include "./XmlReader/tinystr.h"
#include<map>


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

	//virtual shared_ptr<Camera> GetCamera() { return this->camera; }

	//virtual void SetCamera(shared_ptr<Camera> camera) { this->camera = camera; }

	virtual Vector2 GetBound();

	virtual shared_ptr<CTileSet> GetTileSetByTileID(int id);

	virtual void AddTileSet(int firstgid, shared_ptr<CTileSet> tileSet);

	virtual void AddLayer(shared_ptr<CMapLayer> layer);

	void UpdateCamPosition(Vector2 newPos);
	Vector2 ConvertToPositionInCam(Vector2 oldPos);

	virtual void Update(int dt);

	virtual void Render();



	static shared_ptr<CGameMap> FromTMX(string filePath);
	~CGameMap();
};

typedef CGameMap* LPGameMap;

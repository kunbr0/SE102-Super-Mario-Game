#pragma once
#include "Utils.h"
#include "TileSet.h"
#include "MapLayer.h"
#include "./XmlReader/tinyxml.h"
#include "./XmlReader/tinystr.h"
#include "GameObject.h"

struct kTilesetRender {
	CTileSet* tileset;
	Vector2 rangeId = Vector2(0, 0);
};

class CGameMap
{
	int width;
	int height;
	int tileWidth;
	int tileHeight;
	Vector2 camPosition;
	

	vector<CTileSet*> tilesets;
	kTilesetRender kRender;

	vector<CMapLayer*> layers;

public:
	CGameMap();
	CGameMap(int width, int height, int tileWidth, int tileHeight);


	virtual Vector2 GetBound();

	virtual CTileSet* GetTileSetByTileID(int id);

	virtual void AddTileSet(int firstgid, CTileSet* tileSet);

	virtual void AddLayer(CMapLayer* layer);

	void UpdateCamPosition(Vector2 newPos);
	Vector2 ConvertToPositionInCam(Vector2 oldPos);

	virtual void Update(int dt);

	virtual void Render(float bottomMargin = 0);

	virtual void GetMapSize(Vector2 &out);

	static CGameMap* FromTMX(string, vector<LPGAMEOBJECT>* = NULL, vector<LPGAMEOBJECT>* = NULL, vector<LPGAMEOBJECT>* = NULL);
	~CGameMap();
};

typedef CGameMap* LPGameMap;

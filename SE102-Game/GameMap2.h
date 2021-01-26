#pragma once
#include "Utils.h"
#include "TileSet.h"
#include "MapLayer.h"
#include "./XmlReader/tinyxml.h"
#include "./XmlReader/tinystr.h"
#include "GameObject.h"
#include <unordered_map>
#include "SelectionNode.h"

#include "Grid.h"

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

	CGrid grid;


	vector<CTileSet*> tilesets;
	kTilesetRender kRender;

	vector<CMapLayer*> layers;

public:
	CGameMap();

	virtual Vector2 GetBound();

	virtual CTileSet* GetTileSetByTileID(int id);

	virtual void AddTileSet(int firstgid, CTileSet* tileSet);

	virtual void AddLayer(CMapLayer* layer);

	void UpdateCamPosition(Vector2 newPos);
	Vector2 ConvertToPositionInCam(Vector2 oldPos);

	virtual void Update(int dt);

	virtual void Render(float bottomMargin = 0);

	virtual void GetMapSize(Vector2& out);
	
	CGrid* GetGrid() { return &grid; }

	virtual Vector2 GetMapSize();

	// PlayScene
	static CGameMap* FromTMX(std::string, LPGAMEOBJECT*, vector<LPGAMEOBJECT>*);

	// SelectionScene
	static CGameMap* FromTMX(std::string, vector<LPGAMEOBJECT>*, std::unordered_map<std::string, CSelectionNode*>*);

	// IntroScene
	static CGameMap* FromTMX(std::string, vector<LPGAMEOBJECT>*);

	~CGameMap();
};

typedef CGameMap* LPGameMap;
